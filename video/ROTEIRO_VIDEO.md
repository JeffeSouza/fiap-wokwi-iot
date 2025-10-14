# ROTEIRO PARA VÍDEO DEMONSTRATIVO
## Sistema de Irrigação Inteligente para Café

**Duração Total:** 5 minutos
**Autor:** Jefferson de Souza Oliveira - RM568138
**Plataforma:** Wokwi.com

---

## PREPARAÇÃO ANTES DE GRAVAR

### Checklist:
- [ ] Abrir projeto no Wokwi
- [ ] Testar simulação (verificar se funciona)
- [ ] Preparar Monitor Serial aberto
- [ ] Testar áudio do microfone
- [ ] Preparar software de gravação (OBS Studio, Loom, ou gravador de tela)
- [ ] Ensaiar fala pelo menos 1 vez

### Dicas de Gravação:
- Fale com calma e clareza
- Use o conhecimento pessoal com café (sua vivência!)
- Se errar, pode pausar e regravar a parte
- Não precisa ser perfeito, seja autêntico
- Mostre entusiasmo pelo projeto

---

## CENA 1: ABERTURA E APRESENTAÇÃO (45 segundos)

### O que mostrar na tela:
- Tela inicial do Wokwi com o projeto aberto
- Pode mostrar rapidamente o circuito

### O que falar:

"Olá! Meu nome é Jefferson de Souza Oliveira, RM 568138, e este é meu projeto de IoT para a Fase 2 do curso de Inteligência Artificial da FIAP.

Desenvolvi um sistema de irrigação inteligente para cultivo de café, utilizando ESP32 e simulação no Wokwi. A escolha do café não foi aleatória: durante minha infância, morei no interior e trabalhei diretamente com o plantio, colheita e secagem do café, então conheço na prática os desafios dessa cultura.

O objetivo deste sistema é monitorar as condições do solo e automatizar a irrigação para garantir as condições ideais para o desenvolvimento do café."

---

## CENA 2: APRESENTAÇÃO DOS COMPONENTES (60 segundos)

### O que mostrar na tela:
- Zoom no circuito do Wokwi
- Apontar (com mouse ou seta) cada componente conforme falar

### O que falar:

"Vamos conhecer os componentes do sistema:"

**[Apontar para o ESP32]**
"No centro temos o ESP32, o microcontrolador que processa todas as informações e toma as decisões."

**[Apontar para os 3 botões verdes]**
"Estes três botões verdes simulam sensores de NPK: Nitrogênio, Fósforo e Potássio. Quando pressionados, indicam que o nutriente está presente no solo. Esses nutrientes são essenciais para a produtividade do café."

**[Apontar para o LDR]**
"Este LDR, ou fotoresistor, simula um sensor de pH do solo. O sistema converte a leitura analógica para uma escala de pH de 0 a 14. Para o café, o ideal é entre 5.5 e 6.5, ou seja, levemente ácido."

**[Apontar para o DHT22]**
"O DHT22 aqui simula um sensor de umidade do solo. Ele fornece a porcentagem de umidade, sendo que para café o ideal é manter entre 60% e 80%."

**[Apontar para o Relé]**
"E por fim, temos o módulo relé, que controla a bomba d'água. Quando as condições são favoráveis, o sistema liga automaticamente a irrigação."

---

## CENA 3: EXPLICAÇÃO DA LÓGICA (60 segundos)

### O que mostrar na tela:
- Pode abrir o código brevemente OU
- Manter o circuito e explicar verbalmente

### O que falar:

"A lógica de decisão do sistema é simples, mas eficaz:"

"O sistema faz leituras a cada 3 segundos e verifica três condições principais:

Primeira: O solo está seco? Ou seja, a umidade está abaixo de 60%?

Segunda: O pH está adequado? Entre 5.5 e 6.5, que é o ideal para café?

Terceira: Todos os nutrientes NPK estão presentes?

Se TODAS essas condições forem verdadeiras, o sistema liga a bomba d'água automaticamente.

Por outro lado, se o solo estiver encharcado - acima de 80% de umidade - ou se o pH estiver fora do ideal, ou se faltar algum nutriente, o sistema desliga ou mantém a bomba desligada.

Além disso, o sistema também fornece recomendações. Por exemplo, se o pH estiver muito ácido, ele sugere aplicar calcário. Se faltar potássio, sugere fertilizante potássico."

---

## CENA 4: DEMONSTRAÇÃO PRÁTICA - CENÁRIO 1 (45 segundos)

### O que mostrar na tela:
- Monitor Serial aberto e visível
- Circuito Wokwi rodando

### O que fazer:
1. Iniciar simulação
2. Pressionar os 3 botões (N, P, K)
3. Ajustar DHT22 para umidade baixa (<60%)
4. Ajustar LDR para pH ideal (~6.0)

### O que falar:

"Vamos ver o sistema em ação. Vou iniciar a simulação."

**[Clicar em Start Simulation]**

"Aqui no Monitor Serial podemos ver as leituras em tempo real. Vou simular uma condição onde o solo está seco e precisa de irrigação."

**[Pressionar os 3 botões]**

"Pressiono os três botões indicando que NPK está presente...
Ajusto o LDR para simular pH adequado, por volta de 6.0...
E o DHT22 está indicando umidade baixa, solo seco..."

**[Aguardar sistema processar]**

"Vejam: o sistema analisou todas as condições e decidiu ligar a bomba d'água! Aqui está a mensagem: 'LIGANDO BOMBA D'ÁGUA'. O relé foi acionado e a irrigação começa automaticamente."

---

## CENA 5: DEMONSTRAÇÃO PRÁTICA - CENÁRIO 2 (45 segundos)

### O que mostrar na tela:
- Continuar com Monitor Serial aberto

### O que fazer:
1. Ajustar DHT22 para umidade alta (>80%)
2. Ou soltar um dos botões NPK

### O que falar:

"Agora vou demonstrar o que acontece quando as condições não são ideais."

**[Ajustar umidade para alta OU soltar um botão]**

**Opção A - Umidade Alta:**
"Vou aumentar a umidade do solo acima de 80%, simulando solo encharcado..."

**[Aguardar processamento]**

"Vejam: o sistema detectou solo encharcado e desligou a bomba automaticamente. Além disso, ele fornece recomendações: melhorar drenagem e reduzir frequência de irrigação."

**OU Opção B - Falta de Nutriente:**
"Vou soltar o botão de Potássio, simulando falta desse nutriente..."

**[Aguardar processamento]**

"O sistema detectou a falta de potássio e desligou a bomba. Ele também recomenda aplicar fertilizante potássico antes de continuar irrigando."

---

## CENA 6: CENÁRIO 3 - pH INADEQUADO (30 segundos)

### O que fazer:
1. Voltar condições NPK e umidade OK
2. Ajustar LDR para pH baixo (<5.5) ou alto (>6.5)

### O que falar:

"Último cenário: vou simular um pH inadequado para o café."

**[Ajustar LDR para pH fora do ideal]**

"Com o pH muito ácido, abaixo de 5.5, o sistema reconhece que não é seguro irrigar nessas condições, pois a água não seria absorvida adequadamente pela planta. Ele mantém a bomba desligada e recomenda aplicar calcário para corrigir o pH."

---

## CENA 7: CÓDIGO E CONSIDERAÇÕES (45 segundos)

### O que mostrar na tela:
- Pode mostrar rapidamente o código Arduino
- Ou voltar para visualização do circuito

### O que falar:

"O código foi desenvolvido em C++ para ESP32, utilizando a biblioteca DHT para leitura do sensor de umidade."

**[Se mostrar código, scrollar brevemente]**

"Todas as leituras, análises e decisões estão documentadas no código, com comentários explicativos. Os valores ideais para café - pH entre 5.5 e 6.5, umidade entre 60% e 80% - estão definidos como constantes no início do programa."

"O sistema também foi pensado para ser educativo: todas as mensagens no Monitor Serial são claras, indicando o status de cada sensor e a decisão tomada."

---

## CENA 8: ENCERRAMENTO (30 segundos)

### O que mostrar na tela:
- Pode voltar para visão geral do projeto
- Ou mostrar sua cara (opcional)

### O que falar:

"Este sistema demonstra como a Internet das Coisas pode revolucionar a agricultura, tornando-a mais eficiente e sustentável. Com o monitoramento automático e a irrigação inteligente, economizamos água, reduzimos desperdícios e garantimos condições ideais para o desenvolvimento do café.

Baseado na minha experiência pessoal com o cultivo de café, sei que um sistema como este faria enorme diferença para pequenos e médios produtores.

Obrigado pela atenção! Este foi Jefferson de Souza Oliveira, RM 568138, apresentando meu projeto de IoT para a FIAP."

**[Pausar simulação]**

**[Fim]**

---

## PÓS-PRODUÇÃO

### Depois de Gravar:

1. **Revisar o Vídeo:**
   - Assistir completamente
   - Verificar se áudio está claro
   - Verificar se todas as demonstrações funcionaram
   - Confirmar que o vídeo tem no máximo 5 minutos

2. **Editar (se necessário):**
   - Cortar partes com erros
   - Adicionar textos explicativos (opcional)
   - Ajustar volume do áudio

3. **Exportar:**
   - Formato: MP4
   - Resolução: 1080p (recomendado) ou 720p (mínimo)
   - Taxa de bits: Automática

4. **Upload no YouTube:**
   - Fazer login na sua conta Google
   - YouTube Studio → Criar → Upload de vídeo
   - **IMPORTANTE:** Configurar como "Não listado"
   - Título sugerido: "Sistema IoT Irrigação Café - FIAP RM568138"
   - Descrição: Cole o texto abaixo

---

## DESCRIÇÃO PARA O YOUTUBE

```
Sistema de Irrigação Inteligente para Café - Projeto IoT

Autor: Jefferson de Souza Oliveira
RM: 568138
Curso: Inteligência Artificial - FIAP
Fase: 2

Este projeto implementa um sistema IoT de irrigação inteligente para cultivo de café, utilizando ESP32 e simulação no Wokwi. O sistema monitora pH, umidade e nutrientes (NPK) do solo, acionando automaticamente a bomba d'água quando necessário.

Componentes:
- ESP32 DevKit V1
- 3 Botões (Sensores NPK)
- 1 LDR (Sensor pH)
- 1 DHT22 (Sensor Umidade)
- 1 Módulo Relé (Bomba d'água)

Tecnologias:
- C/C++ (Arduino)
- Wokwi Simulator
- IoT e Sistemas Embarcados

GitHub: [seu-link-aqui]

#FIAP #IoT #ESP32 #Agricultura #Café #SistemasEmbarcados #Automação
```

---

## APÓS O UPLOAD

1. Copiar o link do vídeo
2. Colar no arquivo `link_youtube.txt` (pasta video/)
3. Atualizar o README.md com o link
4. Tirar screenshot do circuito Wokwi
5. Salvar na pasta `imagens/circuito_wokwi.png`

---

## DICAS EXTRAS

### Se Estiver Nervoso:
- Grave em partes menores e junte depois
- Faça pausas entre as cenas
- Lembre-se: você SABE do que está falando (viveu isso!)
- Professores querem ver seu entendimento, não perfeição

### Recursos para Gravação:
- **OBS Studio** (grátis, profissional)
- **Loom** (grátis, 5 min suficiente)
- **ShareX** (grátis, leve)
- **Windows Game Bar** (Win+G, nativo do Windows)
- **QuickTime** (Mac, nativo)

### Problemas Comuns:

**Áudio baixo:**
- Fale mais perto do microfone
- Aumente volume nas configurações do gravador

**Vídeo travando:**
- Feche programas desnecessários
- Reduza qualidade para 720p

**Muito tempo:**
- Fale um pouco mais rápido
- Reduza pausas entre frases
- Corte repetições na edição

**Muito curto:**
- Detalhe mais as explicações
- Mostre mais cenários de teste
- Explique melhor a lógica

---

## BOA SORTE! 🎬☕

Você consegue! Lembre-se: autenticidade > perfeição.

Sua vivência real com café é seu diferencial! 💪
