/*
 * app.js
 * Interface web do Sistema de Controle de Pousos.
 *
 * Aqui NAO existe logica de fila, lista ou pilha. Toda decisao (quem
 * pousa primeiro, desfazer, refazer...) vem do nucleo em C compilado
 * para WebAssembly (nucleo.js). Este arquivo so:
 *   1. chama as funcoes da ponte (interface_web/ponte_web.c);
 *   2. le o estado do sistema em JSON;
 *   3. desenha as tabelas na pagina.
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

const el = (id) => document.getElementById(id);

let nucleo = null;         // funcoes do C
let estado = null;         // ultima fotografia do sistema
let idSelecionado = null;  // voo marcado na fila de espera

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
}

function desenharProximo() {
  const caixa = el("proximo-voo");
  const proximo = estado.fila[0];
  caixa.replaceChildren();
  if (proximo) {
    caixa.append(criar("span", "proximo-codigo", proximo.codigo), etiqueta(proximo.prioridade));
  } else {
    caixa.append(criar("span", "vazio", "Nenhum voo aguardando"));
  }
  el("botao-pouso").disabled = !proximo;
}

function desenharFila() {
  const corpo = el("corpo-fila");
  corpo.replaceChildren();

  for (const voo of estado.fila) {
    const linha = criar("tr", voo.id === idSelecionado ? "selecionado" : "");

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
      criar("td", "fraco", voo.chegada),
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
    const linha = criar("tr");
    const celulaPrioridade = criar("td");
    celulaPrioridade.append(etiqueta(voo.prioridade));
    linha.append(
      criar("td", "mono", String(estado.historico.length - i)),
      criar("td", "mono", voo.codigo),
      celulaPrioridade,
      criar("td", "fraco", voo.chegada),
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
    for (const voo of doNivel) voos.append(criar("span", "ficha", voo.codigo));

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

function codigoDe(id) {
  const voo = estado.fila.concat(estado.historico).find((v) => v.id === id);
  return voo ? voo.codigo : "id " + id;
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
  atualizar();
  avisar("Voo " + codigo + " registrado como " + prioridade + ".", "ok");
}

function aoAutorizarPouso() {
  const id = nucleo.autorizarPouso();
  if (id === 0) {
    avisar("Não há voos aguardando.", "erro");
    return;
  }
  atualizar();
  const voo = estado.historico[0];
  avisar("Pouso autorizado: " + voo.codigo + " (" + voo.prioridade + ").", "ok");
}

function aoReclassificar() {
  if (idSelecionado === null) return;
  const nova = el("nova-prioridade").value;
  const voo = estado.fila.find((v) => v.id === idSelecionado);
  const codigo = voo.codigo;

  /* Evita empilhar no "desfazer" uma troca que nao muda nada. */
  if (voo.prioridade === nova) {
    avisar("O voo " + codigo + " já é " + nova + ". Escolha outra prioridade.", "erro");
    return;
  }
  if (!nucleo.reclassificar(idSelecionado, nova)) {
    avisar("Não foi possível reclassificar o voo " + codigo + " (limite de correções atingido).", "erro");
    return;
  }
  atualizar();
  avisar("Voo " + codigo + " reclassificado para " + nova + ".", "ok");
}

function aoDesfazer() {
  if (idSelecionado === null) return;
  const codigo = codigoDe(idSelecionado);
  if (!nucleo.desfazer(idSelecionado)) {
    avisar("O voo " + codigo + " não tem reclassificação para desfazer.", "erro");
    return;
  }
  atualizar();
  avisar("Reclassificação do voo " + codigo + " desfeita.", "ok");
}

function aoRefazer() {
  if (idSelecionado === null) return;
  const codigo = codigoDe(idSelecionado);
  if (!nucleo.refazer(idSelecionado)) {
    avisar("O voo " + codigo + " não tem reclassificação para refazer.", "erro");
    return;
  }
  atualizar();
  avisar("Reclassificação do voo " + codigo + " refeita.", "ok");
}

function aoCarregarExemplo() {
  nucleo.iniciar();
  idSelecionado = null;
  for (const [codigo, prioridade] of EXEMPLO) nucleo.registrar(codigo, prioridade);
  atualizar();
  avisar("Exemplo carregado: 8 voos em horário de pico. O mayday chegou em 6º e é o próximo.", "ok");
}

function aoReiniciar() {
  nucleo.iniciar();
  idSelecionado = null;
  atualizar();
  avisar("Sistema reiniciado. Registre um voo para começar.");
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
  el("botao-desfazer").addEventListener("click", aoDesfazer);
  el("botao-refazer").addEventListener("click", aoRefazer);
  el("botao-exemplo").addEventListener("click", aoCarregarExemplo);
  el("botao-reiniciar").addEventListener("click", aoReiniciar);

  atualizar();
  avisar("Pronto. Registre um voo para começar, ou carregue o exemplo.");
}).catch((erro) => {
  avisar("Não foi possível carregar o núcleo em WebAssembly: " + erro, "erro");
});
