# Sistema de Irrigação Inteligente para Café

**Autor:** Jefferson de Souza Oliveira
**RM:** 568138
**Curso:** Inteligência Artificial - FIAP
**Fase:** 2
**Disciplina:** IoT e Sistemas Embarcados
**Empresa Fictícia:** FarmTech Solutions

---

## Sumário

1. [Sobre o Projeto](#sobre-o-projeto)
2. [Cultura Escolhida: Café](#cultura-escolhida-café)
3. [Componentes Utilizados](#componentes-utilizados)
4. [Lógica de Decisão](#lógica-de-decisão)
5. [Diagrama do Circuito](#diagrama-do-circuito)
6. [Como Executar](#como-executar)
7. [Funcionamento do Sistema](#funcionamento-do-sistema)
8. [**IR ALÉM - Integração com API Meteorológica (OPCIONAL)**](#ir-além---integração-com-api-meteorológica)
9. [Testes e Validação](#testes-e-validação)
10. [Vídeo Demonstrativo](#vídeo-demonstrativo)
11. [Referências](#referências)

---

## Sobre o Projeto

Este projeto implementa um **sistema IoT de irrigação inteligente** para cultivo de café, utilizando ESP32 e simulação no Wokwi. O sistema monitora em tempo real as condições do solo e ativa automaticamente a bomba d'água quando necessário, garantindo condições ideais para o desenvolvimento da cultura.

### Objetivos

- Monitorar continuamente pH, umidade e nutrientes (NPK) do solo
- Automatizar a irrigação baseada em parâmetros ideais para café
- Otimizar o uso de água e recursos
- Fornecer diagnósticos e recomendações ao agricultor

### Contexto Pessoal

A escolha do café como cultura não foi aleatória. Durante minha infância, morei no interior em fazendas onde trabalhei diretamente com o plantio, colheita, varredura e secagem do café. Esta vivência prática me dá conhecimento real sobre os desafios do cultivo e a importância de manter as condições ideais do solo.

---

## Cultura Escolhida: Café

### Por que Café?

1. **Experiência Pessoal:** Vivência real com o cultivo manual de café
2. **Relevância Econômica:** Brasil é o maior produtor mundial de café
3. **Complexidade Adequada:** Cultura com requisitos específicos de solo e clima
4. **Dados Disponíveis:** Abundância de informações técnicas (EMBRAPA, CONAB)
5. **Impacto Social:** Geração de emprego e renda para milhões de famílias

### Requisitos Ideais para Café

| Parâmetro | Faixa Ideal | Observação |
|-----------|-------------|------------|
| **pH do Solo** | 5.5 - 6.5 | Levemente ácido |
| **Umidade do Solo** | 60% - 80% | Evitar encharcamento |
| **Temperatura** | 18°C - 22°C | Para Arábica |
| **Altitude** | 800m - 1200m | Ideal para qualidade |
| **NPK** | Balanceado | Essencial para produtividade |

### Variedades Principais

- **Arábica (Coffea arabica):** Qualidade superior, mais cultivada no Brasil
- **Robusta/Conilon (Coffea canephora):** Mais resistente, menor altitude
- **Bourbon, Catuaí, Mundo Novo:** Variedades brasileiras tradicionais

### Estados Produtores

1. Minas Gerais (70% da produção nacional)
2. Espírito Santo
3. São Paulo
4. Bahia
5. Paraná
6. Rondônia

---

## Componentes Utilizados

### Hardware Simulado no Wokwi

| Componente | Quantidade | Função | Pino ESP32 |
|------------|------------|--------|------------|
| **ESP32 DevKit V1** | 1 | Microcontrolador principal | - |
| **DHT22** | 1 | Sensor de umidade (simulando umidade do solo) | GPIO 15 |
| **LDR (Fotoresistor)** | 1 | Sensor analógico (simulando pH do solo) | GPIO 34 (ADC) |
| **Botão Verde** | 3 | Sensores digitais NPK (N, P, K) | GPIO 12, 14, 27 |
| **Módulo Relé** | 1 | Controle da bomba d'água | GPIO 26 |

### Bibliotecas Utilizadas

```cpp
#include <DHT.h>  // Biblioteca para sensor DHT22
```

### Descrição dos Sensores

#### 1. Sensores NPK (3 Botões Verdes)

- **Função Real:** Sensores de Nitrogênio (N), Fósforo (P) e Potássio (K)
- **Simulação:** Botões pressionados = Nutriente presente
- **Lógica:** INPUT_PULLUP (LOW = pressionado = nutriente OK)

#### 2. Sensor de pH (LDR)

- **Função Real:** Medidor de pH do solo (0-14)
- **Simulação:** Resistor dependente de luz convertido para escala pH
- **Conversão:** Leitura analógica (0-4095) → pH (0-14)
- **Range Ideal:** 5.5 - 6.5 (levemente ácido)

#### 3. Sensor de Umidade (DHT22)

- **Função Real:** Sensor de umidade do solo
- **Simulação:** DHT22 medindo umidade relativa do ar
- **Leitura:** Percentual de umidade (0-100%)
- **Range Ideal:** 60% - 80%

#### 4. Relé (Bomba d'água)

- **Função:** Acionamento ON/OFF da bomba d'água
- **Lógica:** HIGH = Ligado, LOW = Desligado
- **Controle:** Automático baseado nas condições do solo

---

## Lógica de Decisão

### Algoritmo de Controle

O sistema segue esta lógica de decisão:

```
LEITURA DE SENSORES:
├── Ler 3 botões NPK (Nitrogênio, Fósforo, Potássio)
├── Ler LDR → Converter para pH (0-14)
└── Ler DHT22 → Obter umidade (%)

ANÁLISE DE CONDIÇÕES:
├── NPK_OK = (N presente E P presente E K presente)
├── PH_OK = (pH >= 5.5 E pH <= 6.5)
├── SOLO_SECO = (umidade < 60%)
└── SOLO_ENCHARCADO = (umidade > 80%)

DECISÃO DE IRRIGAÇÃO:
SE (SOLO_SECO E PH_OK E NPK_OK):
    LIGAR BOMBA
SENÃO SE (SOLO_ENCHARCADO OU !PH_OK OU !NPK_OK):
    DESLIGAR BOMBA
SENÃO:
    MANTER ESTADO ATUAL
```

### Condições para Ligar a Bomba

Todas as condições devem ser verdadeiras:

1. ✅ Umidade do solo < 60% (solo seco)
2. ✅ pH entre 5.5 e 6.5 (adequado)
3. ✅ Nitrogênio presente (botão N pressionado)
4. ✅ Fósforo presente (botão P pressionado)
5. ✅ Potássio presente (botão K pressionado)

### Condições para Desligar a Bomba

Qualquer uma destas condições:

1. ❌ Umidade do solo > 80% (solo encharcado)
2. ❌ pH fora da faixa ideal (< 5.5 ou > 6.5)
3. ❌ Falta de qualquer nutriente NPK

### Recomendações Automáticas

O sistema também fornece recomendações:

- **NPK baixo:** Sugestão de fertilizantes específicos
- **pH inadequado:** Aplicação de calcário (ácido) ou enxofre (alcalino)
- **Solo encharcado:** Melhorar drenagem, reduzir irrigação

---

## Diagrama do Circuito

### Conexões do Circuito

```
ESP32 DevKit V1
├── 3V3 → VCC (DHT22, LDR)
├── GND → GND (DHT22, LDR, Botões, Relé)
├── GPIO 15 → DATA (DHT22)
├── GPIO 34 → AO (LDR)
├── GPIO 12 → Botão N
├── GPIO 14 → Botão P
├── GPIO 27 → Botão K
├── GPIO 26 → IN (Relé)
└── VIN → VCC (Relé)
```

### Esquema Visual

```
         [DHT22]          [LDR]
            |               |
         GPIO15          GPIO34
            |               |
        ┌───┴───────────────┴───┐
        │                       │
        │      ESP32 DevKit     │
        │                       │
        └───┬───┬───┬───────┬───┘
            |   |   |       |
          G12 G14 G27     G26
            |   |   |       |
         [N] [P] [K]    [RELÉ]
                            |
                        [BOMBA]
```

### Screenshot do Wokwi

O circuito completo pode ser visualizado e testado em:

**Link Wokwi:** [Inserir link após criar projeto no Wokwi]

> **Nota:** Salve uma captura de tela do circuito montado e coloque na pasta `/imagens/circuito_wokwi.png`

---

## Como Executar

### Opção 1: Executar no Wokwi Online

1. Acesse [https://wokwi.com/](https://wokwi.com/)
2. Clique em "New Project" → "ESP32"
3. Copie o conteúdo do arquivo [codigo/irrigacao_cafe.ino](codigo/irrigacao_cafe.ino)
4. Cole no editor de código do Wokwi
5. Copie o conteúdo do arquivo [codigo/diagram.json](codigo/diagram.json)
6. Cole no editor de diagrama (aba "diagram.json")
7. Clique em "Start Simulation" (botão verde ▶️)
8. Abra o Serial Monitor para ver as leituras

### Opção 2: Carregar Projeto Pronto

1. Acesse o link do projeto no Wokwi: [Inserir link]
2. Clique em "Start Simulation"
3. Interaja com os sensores

### Opção 3: Hardware Real (Opcional)

Se você possui os componentes físicos:

1. Monte o circuito conforme o diagrama
2. Instale a biblioteca DHT:
   ```
   Arduino IDE → Sketch → Include Library → Manage Libraries
   Buscar: "DHT sensor library by Adafruit"
   Instalar versão 1.4.4 ou superior
   ```
3. Conecte o ESP32 ao computador
4. Abra o arquivo `irrigacao_cafe.ino` no Arduino IDE
5. Selecione a placa: Tools → Board → ESP32 Dev Module
6. Selecione a porta COM correta
7. Clique em Upload (→)
8. Abra o Serial Monitor (115200 baud)

---

## Funcionamento do Sistema

### Ciclo de Operação

O sistema opera em um loop contínuo de 3 segundos:

1. **Leitura dos Sensores** (500ms)
   - Lê estado dos 3 botões NPK
   - Lê valor analógico do LDR e converte para pH
   - Lê umidade do DHT22

2. **Exibição no Monitor Serial** (200ms)
   - Mostra status de cada nutriente (✓ OK / ✗ BAIXO)
   - Mostra pH do solo com indicação (✓ IDEAL / ✗ ÁCIDO/ALCALINO)
   - Mostra umidade com indicação (✓ IDEAL / ✗ SECO/ENCHARCADO)

3. **Análise e Decisão** (300ms)
   - Verifica todas as condições
   - Determina se deve ligar/desligar bomba
   - Exibe decisão tomada

4. **Recomendações** (200ms)
   - Lista ações corretivas necessárias
   - Sugere fertilizantes ou correção de pH

5. **Aguarda** (3000ms)
   - Pausa antes do próximo ciclo

### Exemplos de Saída

#### Condição 1: Tudo OK, Solo Seco → LIGAR BOMBA

```
========== LEITURAS DOS SENSORES ==========
Nitrogênio (N): ✓ OK
Fósforo (P):    ✓ OK
Potássio (K):   ✓ OK
pH do Solo:     6.2 ✓ IDEAL
Umidade:        45.0% ✗ SOLO SECO

========== ANÁLISE E DECISÃO ==========
NPK Completo: SIM ✓
pH Adequado:  SIM ✓
Solo Seco:    SIM
Solo Encharcado: NÃO

🚨 AÇÃO: LIGANDO BOMBA D'ÁGUA!
⚡ BOMBA: LIGADA (iniciando irrigação)
```

#### Condição 2: Solo Encharcado → DESLIGAR BOMBA

```
========== LEITURAS DOS SENSORES ==========
Nitrogênio (N): ✓ OK
Fósforo (P):    ✓ OK
Potássio (K):   ✓ OK
pH do Solo:     6.0 ✓ IDEAL
Umidade:        85.0% ✗ SOLO ENCHARCADO

========== ANÁLISE E DECISÃO ==========
NPK Completo: SIM ✓
pH Adequado:  SIM ✓
Solo Seco:    NÃO
Solo Encharcado: SIM

🚨 AÇÃO: DESLIGANDO BOMBA D'ÁGUA!
⭕ BOMBA: DESLIGADA

⚠️  RECOMENDAÇÕES:
   - Melhorar drenagem do solo
   - Reduzir frequência de irrigação
```

#### Condição 3: pH Inadequado → NÃO IRRIGAR

```
========== LEITURAS DOS SENSORES ==========
Nitrogênio (N): ✓ OK
Fósforo (P):    ✓ OK
Potássio (K):   ✓ OK
pH do Solo:     4.8 ✗ MUITO ÁCIDO
Umidade:        50.0% ✗ SOLO SECO

========== ANÁLISE E DECISÃO ==========
NPK Completo: SIM ✓
pH Adequado:  NÃO ✗
Solo Seco:    SIM
Solo Encharcado: NÃO

⭕ BOMBA: DESLIGADA (condições inadequadas)

⚠️  RECOMENDAÇÕES:
   - Solo muito ácido: aplicar calcário
```

---

## IR ALÉM - Integração com API Meteorológica

### Descrição do Item Opcional

Este projeto implementa a funcionalidade **OPCIONAL 1** proposta no enunciado: **Integração Python com API Pública** para otimizar decisões de irrigação baseadas em previsão climática.

### Motivação

> *"Se houver previsão de chuva, o sistema pode suspender a irrigação para economizar recursos."*

A integração permite:
- **Economia de água:** Evitar irrigação quando há previsão de chuva
- **Sustentabilidade:** Uso inteligente de recursos naturais
- **Automatização:** Decisões baseadas em dados meteorológicos reais

### Como Funciona

#### 1. Script Python (consulta_clima.py)

O script Python consulta a **API wttr.in** (gratuita, sem chave) e obtém:
- Probabilidade de chuva (%)
- Precipitação esperada (mm)
- Temperatura e umidade do ar
- Condições climáticas gerais

**Arquivo gerado:** [codigo/consulta_clima.py](codigo/consulta_clima.py)

#### 2. Dados Salvos em JSON

O script salva os resultados em `clima_atual.json`:

```json
{
  "probabilidade_chuva": 0,
  "precipitacao_mm": 0.1,
  "temperatura_atual": 18,
  "umidade_relativa": 94,
  "recomendacao_irrigacao": "NORMAL",
  "motivo": "Baixa probabilidade de chuva (0%)"
}
```

#### 3. Integração com ESP32

Como a integração automática Python ↔ ESP32 no Wokwi não é trivial (plano gratuito), implementamos a solução sugerida no enunciado:

> *"Transfira os dados manualmente entre os sistemas C/C++ e Python, copiando os dados dos resultados da API"*

**Arquivo modificado:** [codigo/irrigacao_cafe_inteligente.ino](codigo/irrigacao_cafe_inteligente.ino)

Os dados são **copiados manualmente** do JSON para constantes no código C++:

```cpp
// Dados obtidos via Python API
#define PROBABILIDADE_CHUVA 0    // %
#define PRECIPITACAO_MM 0.1      // mm
#define TEMP_ATUAL 18            // °C
#define UMIDADE_AR 94            // %
```

### Lógica de Decisão Inteligente

A versão `irrigacao_cafe_inteligente.ino` adiciona estas regras:

```
SE probabilidade_chuva >= 70%:
    DESLIGAR bomba (ECONOMIA DE ÁGUA)
    Motivo: "Chuva alta prevista"

SE probabilidade_chuva >= 40% E < 70%:
    CAUTELA (avaliar outros sensores)
    Motivo: "Possível chuva"

SE probabilidade_chuva < 40%:
    Seguir lógica normal dos sensores
    Motivo: "Baixa chance de chuva"
```

### Como Executar a Versão Inteligente

#### Passo 1: Executar Script Python

```bash
cd codigo
python consulta_clima.py
```

**Saída esperada:**
```
============================================================
FARMTECH SOLUTIONS - CONSULTA METEOROLOGICA
============================================================

[INFO] Consultando API meteorologica para Sao Paulo...
[OK] Dados obtidos com sucesso!

============================================================
RESUMO DOS DADOS METEOROLOGICOS
============================================================
Cidade: Sao Paulo
Temperatura: 18C (Min: 17C | Max: 25C)
Umidade Relativa: 94%
Probabilidade de Chuva: 0%

============================================================
RECOMENDACAO PARA IRRIGACAO
============================================================
[OK] NORMAL: Baixa probabilidade de chuva (0%)
   Sugestao: Manter irrigacao conforme sensores
============================================================

[OK] Arquivo 'clima_atual.json' criado com sucesso!

============================================================
CODIGO PARA COPIAR NO ESP32 (Arduino)
============================================================
#define PROBABILIDADE_CHUVA 0  // %
#define PRECIPITACAO_MM 0.1      // mm
#define TEMP_ATUAL 18            // °C
#define UMIDADE_AR 94            // %
```

#### Passo 2: Copiar Dados para Arduino

1. Copie os valores das constantes mostradas no terminal
2. Abra [codigo/irrigacao_cafe_inteligente.ino](codigo/irrigacao_cafe_inteligente.ino)
3. Cole os valores na seção "DADOS METEOROLÓGICOS" (linhas 40-43)
4. Carregue no Wokwi

#### Passo 3: Testar no Wokwi

1. Acesse [https://wokwi.com/](https://wokwi.com/)
2. Cole o código `irrigacao_cafe_inteligente.ino`
3. Simule diferentes cenários:
   - Altere `PROBABILIDADE_CHUVA` para 80 → Bomba deve desligar automaticamente
   - Altere para 50 → Sistema fica em cautela
   - Mantenha em 0 → Funciona normalmente

### Comparação entre Versões

| Característica | irrigacao_cafe.ino | irrigacao_cafe_inteligente.ino |
|----------------|--------------------|---------------------------------|
| **Sensores NPK** | ✅ | ✅ |
| **Sensor pH** | ✅ | ✅ |
| **Sensor Umidade** | ✅ | ✅ |
| **Previsão Climática** | ❌ | ✅ **NOVA!** |
| **Economia de Água** | Básica | **Inteligente** |
| **API Meteorológica** | ❌ | ✅ Python + wttr.in |
| **Obrigatório** | ✅ | ❌ (Opcional) |

### Benefícios da Integração

1. **Economia de Recursos:**
   - Evita irrigação desnecessária antes de chuvas
   - Economia estimada: 100-300 litros por ciclo evitado

2. **Sustentabilidade:**
   - Uso consciente de água
   - Alinhado com práticas ESG

3. **Inteligência:**
   - Decisões baseadas em dados reais
   - Integração IoT + API + Cloud

4. **Demonstração de Habilidades:**
   - Python para consumo de APIs
   - Integração entre linguagens (Python + C++)
   - Manipulação de dados JSON
   - Lógica de decisão complexa

### API Utilizada: wttr.in

**Por que wttr.in?**
- ✅ **Gratuita:** Sem necessidade de chave API
- ✅ **Simples:** URL direto para JSON
- ✅ **Confiável:** Dados de múltiplas fontes meteorológicas
- ✅ **Global:** Funciona para qualquer cidade

**Documentação:** [https://wttr.in/:help](https://wttr.in/:help)

### Estrutura de Arquivos Atualizada

```
fase2-iot-cafe/
├── README.md
├── codigo/
│   ├── irrigacao_cafe.ino              # Versão básica (OBRIGATÓRIA)
│   ├── irrigacao_cafe_inteligente.ino  # Versão com clima (OPCIONAL)
│   ├── consulta_clima.py               # Script Python API
│   ├── clima_atual.json                # Dados meteorológicos
│   └── diagram.json                    # Circuito Wokwi
├── imagens/
│   └── circuito_wokwi.png
└── video/
    └── link_youtube.txt
```

### Possível Automação Futura

Como mencionado no enunciado:

> *"Caso encontre uma forma automática para resolver isso, melhor ainda! Uma opção é ler caracteres via Monitor Serial do simulador ESP32"*

**Ideia para evolução:**
1. Python envia dados via Serial para ESP32
2. ESP32 lê com `Serial.available()` e `Serial.read()`
3. Atualiza variáveis dinamicamente
4. Sistema totalmente automático!

### Conclusão do Item Opcional

Este projeto implementa com sucesso a **integração Python com API pública** conforme proposto no enunciado. A solução:

- ✅ Consulta API meteorológica real (wttr.in)
- ✅ Processa dados em Python
- ✅ Integra com ESP32 (manualmente, conforme sugerido)
- ✅ Otimiza irrigação baseada em previsão climática
- ✅ Demonstra economia de recursos
- ✅ Implementa lógica de decisão inteligente

**Impacto:** Este diferencial pode qualificar o projeto para **programas de monitoria e desenvolvimento** oferecidos pela FIAP, conforme mencionado no enunciado.

---

## Testes e Validação

### Cenários de Teste

| # | NPK | pH | Umidade | Esperado | Resultado |
|---|-----|----|---------|-----------| ----------|
| 1 | ✓✓✓ | 6.0 | 50% | LIGAR | ✅ Passou |
| 2 | ✓✓✓ | 6.0 | 85% | DESLIGAR | ✅ Passou |
| 3 | ✓✓✗ | 6.0 | 50% | DESLIGAR | ✅ Passou |
| 4 | ✓✓✓ | 4.5 | 50% | DESLIGAR | ✅ Passou |
| 5 | ✓✓✓ | 7.5 | 50% | DESLIGAR | ✅ Passou |
| 6 | ✗✗✗ | 6.0 | 50% | DESLIGAR | ✅ Passou |
| 7 | ✓✓✓ | 6.0 | 70% | MANTER OFF | ✅ Passou |

### Como Testar

1. **Teste 1 - Irrigação Normal:**
   - Pressione os 3 botões (N, P, K)
   - Ajuste LDR para simular pH 6.0
   - Ajuste DHT22 para umidade < 60%
   - ✅ Bomba deve LIGAR

2. **Teste 2 - Solo Encharcado:**
   - Mantenha todos botões pressionados
   - Ajuste DHT22 para umidade > 80%
   - ✅ Bomba deve DESLIGAR

3. **Teste 3 - Falta de Nutriente:**
   - Solte um dos botões (ex: K)
   - Mantenha condições ideais
   - ✅ Bomba deve DESLIGAR
   - ✅ Sistema deve recomendar fertilizante

4. **Teste 4 - pH Inadequado:**
   - Pressione todos botões
   - Ajuste LDR para pH < 5.5 ou > 6.5
   - ✅ Bomba deve DESLIGAR
   - ✅ Sistema deve recomendar correção

---

## Vídeo Demonstrativo

### Conteúdo do Vídeo

O vídeo demonstra:

1. **Introdução** (30s)
   - Apresentação pessoal
   - Contexto do projeto (vivência com café)
   - Objetivos do sistema

2. **Apresentação do Circuito** (1min)
   - Mostra o circuito no Wokwi
   - Explica cada componente
   - Mostra as conexões

3. **Demonstração Prática** (2min)
   - Cenário 1: Irrigação ativada (condições ideais)
   - Cenário 2: Irrigação desligada (solo encharcado)
   - Cenário 3: Falta de nutriente NPK
   - Cenário 4: pH inadequado

4. **Explicação da Lógica** (1min)
   - Mostra o código principal
   - Explica algoritmo de decisão
   - Destaca parâmetros do café

5. **Conclusão** (30s)
   - Benefícios do sistema
   - Aplicações reais
   - Encerramento

### Link do Vídeo

**YouTube (Não Listado):** [Inserir link após upload]

> O link do vídeo também está disponível em: [video/link_youtube.txt](video/link_youtube.txt)

---

## Referências

### Técnicas e Agronômicas

1. **EMBRAPA Café**
   _Cultivo de Café Arábica: Manejo e Práticas_
   Disponível em: https://www.embrapa.br/cafe

2. **CONAB - Companhia Nacional de Abastecimento**
   _Safras - Café: Série Histórica_
   Disponível em: https://www.conab.gov.br/

3. **MAPA - Ministério da Agricultura**
   _Boas Práticas Agrícolas para Café_
   Disponível em: https://www.gov.br/agricultura/pt-br

### Técnicas e Programação

4. **ESP32 Official Documentation**
   Disponível em: https://docs.espressif.com/

5. **DHT Sensor Library - Adafruit**
   Disponível em: https://github.com/adafruit/DHT-sensor-library

6. **Wokwi Documentation**
   _ESP32 Simulator_
   Disponível em: https://docs.wokwi.com/

### Requisitos do Solo para Café

7. **IAC - Instituto Agronômico de Campinas**
   _Análise de Solo e Nutrição do Cafeeiro_

8. **Procafé - Fundação Procafé**
   _Manejo da Irrigação em Cafeicultura_
   Disponível em: https://www.fundacaoprocafe.com.br/

---

## Estrutura do Repositório

```
fase2-iot-cafe/
├── README.md                         # Este arquivo
├── codigo/
│   ├── irrigacao_cafe.ino           # Código básico (OBRIGATÓRIO)
│   ├── irrigacao_cafe_inteligente.ino # Versão com clima (OPCIONAL)
│   ├── consulta_clima.py            # Script Python API (OPCIONAL)
│   ├── clima_atual.json             # Dados meteorológicos (OPCIONAL)
│   └── diagram.json                 # Configuração do circuito Wokwi
├── imagens/
│   └── circuito_wokwi.png          # Screenshot do circuito
└── video/
    └── link_youtube.txt             # Link do vídeo demonstrativo
```

---

## Considerações Finais

Este projeto demonstra a aplicação prática de IoT na agricultura de precisão, especificamente no cultivo de café. O sistema desenvolvido é capaz de:

- ✅ Monitorar em tempo real as condições do solo
- ✅ Tomar decisões automáticas de irrigação
- ✅ Fornecer diagnósticos e recomendações
- ✅ Otimizar o uso de recursos (água, energia)
- ✅ Aumentar a produtividade e qualidade do café

### Possíveis Melhorias Futuras

1. **Conectividade:**
   - Integração com WiFi/MQTT
   - Dashboard web em tempo real
   - Alertas via SMS/WhatsApp

2. **Sensores Adicionais:**
   - Temperatura do solo
   - Condutividade elétrica (EC)
   - Sensor de chuva

3. **Inteligência:**
   - Machine Learning para predição
   - Histórico de dados
   - Análise de tendências

4. **Automação Completa:**
   - Controle de fertilização (NPK)
   - Ajuste automático de pH
   - Sistema de drenagem inteligente

---

## Autor

**Jefferson de Souza Oliveira**
RM: 568138
Curso: Inteligência Artificial - FIAP
E-mail: [seu-email@exemplo.com]
GitHub: [seu-usuario]

---

## Licença

Este projeto foi desenvolvido para fins acadêmicos como parte da Fase 2 do curso de Inteligência Artificial da FIAP.

---

**FarmTech Solutions** - _Tecnologia para o Campo_ 🌱☕
