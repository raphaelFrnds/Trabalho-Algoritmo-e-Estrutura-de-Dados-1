/*
 * app.js
 * Interface web do Sistema de Controle de Pousos.
 *
 * Aqui NAO existe logica de fila, lista ou pilha. Toda decisao (quem
 * pousa primeiro, desfazer, refazer...) vem do nucleo em C compilado
 * para WebAssembly (nucleo.js). Este arquivo so:
 *   1. chama as funcoes da ponte (interface_web/ponte_web.c);
 *   2. le o estado do sistema em JSON;
 *   3. desenha as tabelas na pagina;
 *   4. anota no "Registro de operacoes" qual funcao do C foi usada.
 */
"use strict";

/* Textos com acento para exibicao; a chave e o nome usado no C. */
const DESCRICOES = {
  mayday: "Emergência (MAYDAY)",
  panpan: "Urgência (PAN-PAN)",
  combustivel: "Combustível mínimo",
  regular: "Voo regular",
  treinamento: "Treinamento / translado",
};

/* A mesma chegada de 8 voos da simulacao do Modulo 5. */
const EXEMPLO = [
  ["AZU4521", "regular"], ["GLO1180", "combustivel"], ["PTB2210", "treinamento"],
  ["TAM3301", "panpan"], ["AZU4700", "combustivel"], ["GLO1555", "mayday"],
  ["TAM3090", "regular"], ["AZU4888", "panpan"],
];

const MAX_REGISTRO = 30;

const el = (id) => document.getElementById(id);

let nucleo = null;              // funcoes do C
let estado = null;              // ultima fotografia do sistema
let idSelecionado = null;       // voo marcado na fila de espera
let destaque = new Set();       // ids que acabaram de mudar (piscam uma vez)
let idFrenteAnterior = null;    // para piscar o "Proximo a pousar" quando ele troca

/* ---------- Pequenos ajudantes de tela ---------- */

function criar(tag, classe, texto) {
  const no = document.createElement(tag);
  if (classe) no.className = classe;
  if (texto !== undefined) no.textContent = texto;
  return no;
}

function etiqueta(prioridade) {
  return criar("span", "etiqueta p-" + prioridade, prioridade);
}

/* O id do voo e a sua ordem de chegada (o contador reinicia com o sistema). */
function ordemChegada(voo) {
  return voo.id + "º";
}

function celulaChegada(voo) {
  const celula = criar("td", "chegou", ordemChegada(voo));
  celula.title = "Registrado em " + voo.chegada;
  return celula;
}

function avisar(texto, tipo) {
  const status = el("status");
  status.textContent = texto;
  status.className = "status" + (tipo ? " " + tipo : "");
}

function preencherPrioridades(select, niveis) {
  select.replaceChildren();
  for (const nivel of niveis) {
    const opcao = criar("option", "", DESCRICOES[nivel.nome] || nivel.descricao);
    opcao.value = nivel.nome;
    select.append(opcao);
  }
  select.value = "regular";
}

/* ---------- Registro de operacoes ---------- */

function limparRegistro() {
  el("registro").replaceChildren();
  el("registro-vazio").hidden = false;
}

/* texto: o que aconteceu; custo: complexidade; funcoes: o caminho no C. */
function anotar(texto, custo, funcoes) {
  const lista = el("registro");
  const item = criar("li");
  item.append(
    criar("span", "registro-texto", texto),
    criar("span", "registro-custo", custo),
    criar("span", "registro-codigo", funcoes),
  );
  lista.prepend(item);
  while (lista.children.length > MAX_REGISTRO) lista.lastElementChild.remove();
  el("registro-vazio").hidden = true;
}

/* ---------- Desenho da pagina a partir do estado ---------- */

function atualizar() {
  estado = JSON.parse(nucleo.estado());

  const selecionado = estado.fila.find((voo) => voo.id === idSelecionado) || null;
  if (!selecionado) idSelecionado = null;

  desenharProximo();
  desenharFila();
  desenharHistorico();
  desenharNiveis();
  desenharAcoes(selecionado);

  destaque = new Set();   // o lampejo vale so para esta atualizacao
}

function desenharProximo() {
  const caixa = el("proximo-voo");
  const proximo = estado.fila[0];
  const idFrente = proximo ? proximo.id : null;

  caixa.replaceChildren();
  caixa.classList.remove("mudou");
  if (proximo) {
    caixa.append(criar("span", "proximo-codigo", proximo.codigo), etiqueta(proximo.prioridade));
    if (idFrente !== idFrenteAnterior) {
      void caixa.offsetWidth;          // reinicia a animacao
      caixa.classList.add("mudou");
    }
  } else {
    caixa.append(criar("span", "vazio", "Nenhum voo aguardando"));
  }
  idFrenteAnterior = idFrente;
  el("botao-pouso").disabled = !proximo;
}

function desenharFila() {
  const corpo = el("corpo-fila");
  corpo.replaceChildren();

  for (const voo of estado.fila) {
    const classes = [];
    if (voo.id === idSelecionado) classes.push("selecionado");
    if (destaque.has(voo.id)) classes.push("mudou");
    const linha = criar("tr", classes.join(" "));

    const radio = criar("input");
    radio.type = "radio";
    radio.name = "voo";
    radio.checked = voo.id === idSelecionado;
    radio.setAttribute("aria-label", "Selecionar o voo " + voo.codigo);
    radio.addEventListener("change", () => selecionar(voo.id));
    const celulaRadio = criar("td", "col-sel");
    celulaRadio.append(radio);

    const celulaPrioridade = criar("td");
    celulaPrioridade.append(etiqueta(voo.prioridade));

    linha.append(
      celulaRadio,
      criar("td", "mono", String(voo.posicao)),
      criar("td", "mono", voo.codigo),
      celulaPrioridade,
      celulaChegada(voo),
    );
    linha.addEventListener("click", () => selecionar(voo.id));
    corpo.append(linha);
  }

  el("fila-vazia").hidden = estado.fila.length > 0;
  el("aguardando").textContent = estado.aguardando + " aguardando";
}

function desenharHistorico() {
  const corpo = el("corpo-historico");
  corpo.replaceChildren();

  estado.historico.forEach((voo, i) => {
    const linha = criar("tr", destaque.has(voo.id) ? "mudou" : "");
    const celulaPrioridade = criar("td");
    celulaPrioridade.append(etiqueta(voo.prioridade));
    linha.append(
      criar("td", "mono", String(estado.historico.length - i)),
      criar("td", "mono", voo.codigo),
      celulaPrioridade,
      celulaChegada(voo),
    );
    corpo.append(linha);
  });

  el("historico-vazio").hidden = estado.historico.length > 0;
}

function desenharNiveis() {
  const caixa = el("niveis");
  caixa.replaceChildren();

  estado.niveis.forEach((nivel, i) => {
    const linha = criar("div", "nivel");
    const voos = criar("div", "nivel-voos");
    const doNivel = estado.fila.filter((voo) => voo.nivel === i + 1);

    if (doNivel.length === 0) voos.append(criar("span", "nivel-vazio", "vazia"));
    for (const voo of doNivel) {
      const ficha = criar("span", "ficha" + (destaque.has(voo.id) ? " mudou" : ""), voo.codigo);
      ficha.title = "Chegou em " + ordemChegada(voo);
      voos.append(ficha);
    }

    const rotulo = criar("div");
    rotulo.append(etiqueta(nivel.nome));
    linha.append(criar("span", "nivel-indice", "niveis[" + i + "]"), rotulo, voos);
    caixa.append(linha);
  });
}

function desenharAcoes(selecionado) {
  el("nova-prioridade").disabled = !selecionado;
  el("botao-reclassificar").disabled = !selecionado;
  el("botao-desfazer").disabled = !selecionado || !selecionado.pode_desfazer;
  el("botao-refazer").disabled = !selecionado || !selecionado.pode_refazer;
  el("selecionado").textContent = selecionado
    ? "Voo selecionado: " + selecionado.codigo + " (" + selecionado.prioridade + ")"
    : "Selecione um voo na fila para corrigir a prioridade.";
}

function selecionar(id) {
  idSelecionado = id;
  atualizar();
}

function vooNaFila(id) {
  return estado.fila.find((v) => v.id === id) || null;
}

function indiceNivel(prioridade) {
  return estado.niveis.findIndex((n) => n.nome === prioridade);
}

/* ---------- Acoes do usuario ---------- */

function aoRegistrar(evento) {
  evento.preventDefault();
  const codigo = el("codigo").value.trim().toUpperCase();
  const prioridade = el("prioridade").value;

  const id = nucleo.registrar(codigo, prioridade);
  if (id === 0) {
    avisar("Código inválido: use de 1 a 11 caracteres.", "erro");
    return;
  }
  el("codigo").value = "";
  el("codigo").focus();
  destaque.add(id);
  atualizar();

  const voo = vooNaFila(id);
  avisar("Voo " + codigo + " registrado como " + prioridade + ".", "ok");
  anotar(
    "Registrou " + codigo + " (" + prioridade + "), chegou em " + ordemChegada(voo) +
      ": entrou no fim de niveis[" + indiceNivel(prioridade) + "], posição " + voo.posicao + " da fila.",
    "O(1)",
    "sistema_cadastrar_voo → fila_prio_enfileirar",
  );
}

function aoAutorizarPouso() {
  const id = nucleo.autorizarPouso();
  if (id === 0) {
    avisar("Não há voos aguardando.", "erro");
    return;
  }
  destaque.add(id);
  atualizar();

  const voo = estado.historico[0];
  avisar("Pouso autorizado: " + voo.codigo + " (" + voo.prioridade + ").", "ok");
  anotar(
    "Pouso de " + voo.codigo + " (" + voo.prioridade + "): saiu da frente de niveis[" +
      (voo.nivel - 1) + "] e foi para o início do histórico, sem cópia.",
    "O(k), k = 5",
    "sistema_autorizar_pouso → fila_prio_desenfileirar → lista_simples_inserir(…, 0)",
  );
}

function aoReclassificar() {
  if (idSelecionado === null) return;
  const nova = el("nova-prioridade").value;
  const antes = vooNaFila(idSelecionado);
  const codigo = antes.codigo;

  /* Evita empilhar no "desfazer" uma troca que nao muda nada. */
  if (antes.prioridade === nova) {
    avisar("O voo " + codigo + " já é " + nova + ". Escolha outra prioridade.", "erro");
    return;
  }
  if (!nucleo.reclassificar(idSelecionado, nova)) {
    avisar("Não foi possível reclassificar o voo " + codigo + " (limite de correções atingido).", "erro");
    return;
  }
  destaque.add(idSelecionado);
  atualizar();

  const depois = vooNaFila(idSelecionado);
  avisar("Voo " + codigo + " reclassificado para " + nova + ".", "ok");
  anotar(
    codigo + ": " + antes.prioridade + " → " + nova + " (posição " + antes.posicao + " → " +
      depois.posicao + "). A prioridade antiga foi para a pilha desfazer e a fila foi reconstruída na ordem de chegada.",
    "O(n)",
    "gerenciador_reclassificar → pilha_array_empilhar → reconstruir_fila",
  );
}

/* Desfazer e refazer tem a mesma forma: so mudam a funcao e o texto. */
function aoDesfazerOuRefazer(desfazer) {
  if (idSelecionado === null) return;
  const antes = vooNaFila(idSelecionado);
  const codigo = antes.codigo;
  const ok = desfazer ? nucleo.desfazer(idSelecionado) : nucleo.refazer(idSelecionado);

  if (!ok) {
    avisar("O voo " + codigo + " não tem reclassificação para " + (desfazer ? "desfazer" : "refazer") + ".", "erro");
    return;
  }
  destaque.add(idSelecionado);
  atualizar();

  const depois = vooNaFila(idSelecionado);
  avisar("Reclassificação do voo " + codigo + (desfazer ? " desfeita." : " refeita."), "ok");
  anotar(
    codigo + ": " + antes.prioridade + " → " + depois.prioridade + " (posição " + antes.posicao +
      " → " + depois.posicao + "). " +
      (desfazer ? "Desempilhou da pilha desfazer e guardou a atual na pilha refazer."
                : "Desempilhou da pilha refazer e guardou a atual na pilha desfazer."),
    "O(n)",
    (desfazer ? "gerenciador_desfazer" : "gerenciador_refazer") + " → reconstruir_fila",
  );
}

function aoCarregarExemplo() {
  nucleo.iniciar();
  idSelecionado = null;
  for (const [codigo, prioridade] of EXEMPLO) nucleo.registrar(codigo, prioridade);

  /* Pisca o mayday: chegou em 6º e ja esta na posicao 0. */
  const frente = JSON.parse(nucleo.estado()).fila[0];
  destaque.add(frente.id);
  atualizar();

  limparRegistro();
  avisar("Exemplo carregado: 8 voos em horário de pico. O mayday chegou em 6º e é o próximo.", "ok");
  anotar(
    "Exemplo: 8 voos em horário de pico. " + frente.codigo + " (" + frente.prioridade + ") chegou em " +
      ordemChegada(frente) + " e está na posição 0.",
    "8 × O(1)",
    "sistema_destruir → sistema_iniciar → 8 × sistema_cadastrar_voo",
  );
}

function aoReiniciar() {
  nucleo.iniciar();
  idSelecionado = null;
  atualizar();
  limparRegistro();
  avisar("Sistema reiniciado. Registre um voo para começar.");
  anotar(
    "Sistema reiniciado: a memória de todos os voos foi liberada.",
    "O(n)",
    "sistema_destruir → sistema_iniciar",
  );
}

/* ---------- Partida ---------- */

criarNucleo().then((modulo) => {
  nucleo = {
    iniciar: modulo.cwrap("web_iniciar", null, []),
    registrar: modulo.cwrap("web_registrar", "number", ["string", "string"]),
    autorizarPouso: modulo.cwrap("web_autorizar_pouso", "number", []),
    reclassificar: modulo.cwrap("web_reclassificar", "number", ["number", "string"]),
    desfazer: modulo.cwrap("web_desfazer", "number", ["number"]),
    refazer: modulo.cwrap("web_refazer", "number", ["number"]),
    estado: modulo.cwrap("web_estado", "string", []),
  };

  nucleo.iniciar();
  estado = JSON.parse(nucleo.estado());
  preencherPrioridades(el("prioridade"), estado.niveis);
  preencherPrioridades(el("nova-prioridade"), estado.niveis);

  el("form-voo").addEventListener("submit", aoRegistrar);
  el("botao-pouso").addEventListener("click", aoAutorizarPouso);
  el("botao-reclassificar").addEventListener("click", aoReclassificar);
  el("botao-desfazer").addEventListener("click", () => aoDesfazerOuRefazer(true));
  el("botao-refazer").addEventListener("click", () => aoDesfazerOuRefazer(false));
  el("botao-exemplo").addEventListener("click", aoCarregarExemplo);
  el("botao-reiniciar").addEventListener("click", aoReiniciar);

  atualizar();
  avisar("Pronto. Registre um voo para começar, ou carregue o exemplo.");
}).catch((erro) => {
  avisar("Não foi possível carregar o núcleo em WebAssembly: " + erro, "erro");
});
