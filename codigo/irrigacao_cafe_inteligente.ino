/*
 * SISTEMA DE IRRIGAÇÃO INTELIGENTE PARA CAFÉ - VERSÃO COM PREVISÃO CLIMÁTICA
 * Autor: Jefferson de Souza Oliveira - RM568138
 * Curso: Inteligência Artificial - FIAP Fase 2
 * Empresa Fictícia: FarmTech Solutions
 *
 * DESCRIÇÃO:
 * Sistema IoT que monitora condições do solo para cultivo de café e
 * controla automaticamente a irrigação baseado em:
 * - pH do solo (ideal: 5.5 a 6.5)
 * - Umidade do solo (ideal: 60% a 80%)
 * - Níveis de NPK (Nitrogênio, Fósforo, Potássio)
 * - PREVISÃO CLIMÁTICA (probabilidade de chuva)
 *
 * NOVIDADE - INTEGRAÇÃO COM API METEOROLÓGICA:
 * Este código integra dados de previsão do tempo obtidos via Python
 * da API wttr.in para otimizar decisões de irrigação e economizar água.
 *
 * COMPONENTES:
 * - 3 Botões: Simulam sensores de NPK
 * - 1 LDR: Simula sensor de pH (0-14)
 * - 1 DHT22: Simula sensor de umidade do solo (%)
 * - 1 Relé: Controla bomba d'água
 */

#include <DHT.h>

// ========== DEFINIÇÃO DE PINOS ==========
#define PIN_BTN_N 12        // Botão sensor Nitrogênio (GPIO 12)
#define PIN_BTN_P 14        // Botão sensor Fósforo (GPIO 14)
#define PIN_BTN_K 27        // Botão sensor Potássio (GPIO 27)
#define PIN_LDR 34          // LDR sensor pH (Analógico GPIO 34)
#define PIN_DHT 15          // DHT22 sensor umidade (GPIO 15)
#define PIN_RELE 26         // Relé bomba d'água (GPIO 26)

// ========== PARÂMETROS DO CAFÉ ==========
#define PH_MIN 5.5          // pH mínimo ideal para café
#define PH_MAX 6.5          // pH máximo ideal para café
#define UMIDADE_MIN 60.0    // Umidade mínima ideal (%)
#define UMIDADE_MAX 80.0    // Umidade máxima ideal (%)

// ========== DADOS METEOROLÓGICOS (OBTIDOS VIA PYTHON API) ==========
// Última atualização: 2025-10-11 23:10:21
// Fonte: wttr.in API
// Execute o script: python consulta_clima.py
#define PROBABILIDADE_CHUVA 0    // % (0-100)
#define PRECIPITACAO_MM 0.1      // mm
#define TEMP_ATUAL 18            // °C
#define UMIDADE_AR 94            // %

// Lógica de decisão climática
#define LIMITE_CHUVA_ALTA 70     // >= 70% = Desligar irrigação
#define LIMITE_CHUVA_MODERADA 40 // >= 40% = Reduzir irrigação

// ========== CONFIGURAÇÃO DHT ==========
#define DHTTYPE DHT22
DHT dht(PIN_DHT, DHTTYPE);

// ========== VARIÁVEIS GLOBAIS ==========
bool nitrogenio_ok = false;
bool fosforo_ok = false;
bool potassio_ok = false;
float ph_solo = 0.0;
float umidade_solo = 0.0;
bool bomba_ligada = false;

// ========== SETUP ==========
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("============================================");
  Serial.println("SISTEMA DE IRRIGAÇÃO INTELIGENTE - CAFÉ");
  Serial.println("VERSÃO COM PREVISÃO CLIMÁTICA");
  Serial.println("FarmTech Solutions");
  Serial.println("============================================\n");

  // Configurar pinos de entrada
  pinMode(PIN_BTN_N, INPUT_PULLUP);
  pinMode(PIN_BTN_P, INPUT_PULLUP);
  pinMode(PIN_BTN_K, INPUT_PULLUP);
  pinMode(PIN_LDR, INPUT);

  // Configurar pino de saída
  pinMode(PIN_RELE, OUTPUT);
  digitalWrite(PIN_RELE, LOW); // Bomba inicialmente desligada

  // Inicializar DHT22
  dht.begin();

  Serial.println("Sistema inicializado com sucesso!");
  Serial.println("Monitorando condições do solo...");

  // Exibir dados meteorológicos carregados
  exibirDadosMeteorologicos();

  Serial.println();
  delay(2000);
}

// ========== LOOP PRINCIPAL ==========
void loop() {
  // Ler sensores
  lerSensoresNPK();
  lerSensorPH();
  lerSensorUmidade();

  // Exibir leituras no monitor serial
  exibirLeituras();

  // Avaliar condições e controlar irrigação (COM PREVISÃO CLIMÁTICA)
  controlarIrrigacaoInteligente();

  // Aguardar antes da próxima leitura
  delay(3000);
  Serial.println("----------------------------------------\n");
}

// ========== FUNÇÃO: EXIBIR DADOS METEOROLÓGICOS ==========
void exibirDadosMeteorologicos() {
  Serial.println("\n========== DADOS METEOROLÓGICOS ==========");
  Serial.print("Temperatura Atual: ");
  Serial.print(TEMP_ATUAL);
  Serial.println("°C");

  Serial.print("Umidade do Ar: ");
  Serial.print(UMIDADE_AR);
  Serial.println("%");

  Serial.print("Precipitação: ");
  Serial.print(PRECIPITACAO_MM);
  Serial.println(" mm");

  Serial.print("Probabilidade de Chuva: ");
  Serial.print(PROBABILIDADE_CHUVA);
  Serial.println("%");

  if (PROBABILIDADE_CHUVA >= LIMITE_CHUVA_ALTA) {
    Serial.println("⚠️  ALERTA: Alta probabilidade de chuva!");
    Serial.println("    Sistema economizará água automaticamente");
  } else if (PROBABILIDADE_CHUVA >= LIMITE_CHUVA_MODERADA) {
    Serial.println("⚠️  ATENÇÃO: Possibilidade de chuva");
    Serial.println("    Sistema ajustará irrigação");
  } else {
    Serial.println("✓ Condições climáticas favoráveis");
  }

  Serial.println("==========================================");
}

// ========== FUNÇÃO: LER SENSORES NPK ==========
void lerSensoresNPK() {
  // Botões pressionados = LOW (INPUT_PULLUP invertido)
  // Botão pressionado = Nutriente PRESENTE (OK)
  nitrogenio_ok = (digitalRead(PIN_BTN_N) == LOW);
  fosforo_ok = (digitalRead(PIN_BTN_P) == LOW);
  potassio_ok = (digitalRead(PIN_BTN_K) == LOW);
}

// ========== FUNÇÃO: LER SENSOR pH ==========
void lerSensorPH() {
  // LDR retorna valor analógico (0-4095 no ESP32)
  // Converter para escala de pH (0-14)
  int leitura_ldr = analogRead(PIN_LDR);

  // Mapear 0-4095 para 0-14 (escala de pH)
  ph_solo = map(leitura_ldr, 0, 4095, 0, 140) / 10.0;

  // Garantir que pH fique entre 0 e 14
  if (ph_solo < 0) ph_solo = 0;
  if (ph_solo > 14) ph_solo = 14;
}

// ========== FUNÇÃO: LER SENSOR UMIDADE ==========
void lerSensorUmidade() {
  // DHT22 retorna umidade relativa do ar
  // Estamos simulando como umidade do solo
  umidade_solo = dht.readHumidity();

  // Verificar se leitura é válida
  if (isnan(umidade_solo)) {
    Serial.println("ERRO: Falha ao ler DHT22!");
    umidade_solo = 0.0;
  }
}

// ========== FUNÇÃO: EXIBIR LEITURAS ==========
void exibirLeituras() {
  Serial.println("========== LEITURAS DOS SENSORES ==========");

  // Status NPK
  Serial.print("Nitrogênio (N): ");
  Serial.println(nitrogenio_ok ? "✓ OK" : "✗ BAIXO");

  Serial.print("Fósforo (P):    ");
  Serial.println(fosforo_ok ? "✓ OK" : "✗ BAIXO");

  Serial.print("Potássio (K):   ");
  Serial.println(potassio_ok ? "✓ OK" : "✗ BAIXO");

  // pH do solo
  Serial.print("pH do Solo:     ");
  Serial.print(ph_solo, 1);

  if (ph_solo >= PH_MIN && ph_solo <= PH_MAX) {
    Serial.println(" ✓ IDEAL");
  } else if (ph_solo < PH_MIN) {
    Serial.println(" ✗ MUITO ÁCIDO");
  } else {
    Serial.println(" ✗ MUITO ALCALINO");
  }

  // Umidade do solo
  Serial.print("Umidade:        ");
  Serial.print(umidade_solo, 1);
  Serial.print("%");

  if (umidade_solo >= UMIDADE_MIN && umidade_solo <= UMIDADE_MAX) {
    Serial.println(" ✓ IDEAL");
  } else if (umidade_solo < UMIDADE_MIN) {
    Serial.println(" ✗ SOLO SECO");
  } else {
    Serial.println(" ✗ SOLO ENCHARCADO");
  }

  Serial.println();
}

// ========== FUNÇÃO: CONTROLAR IRRIGAÇÃO INTELIGENTE ==========
void controlarIrrigacaoInteligente() {
  Serial.println("========== ANÁLISE E DECISÃO INTELIGENTE ==========");

  // Verificar se todos os nutrientes estão OK
  bool npk_ok = nitrogenio_ok && fosforo_ok && potassio_ok;

  // Verificar se pH está no range ideal
  bool ph_ok = (ph_solo >= PH_MIN && ph_solo <= PH_MAX);

  // Verificar condições de umidade
  bool solo_seco = (umidade_solo < UMIDADE_MIN);
  bool solo_encharcado = (umidade_solo > UMIDADE_MAX);

  // ========== NOVA LÓGICA: VERIFICAR PREVISÃO CLIMÁTICA ==========
  bool chuva_alta = (PROBABILIDADE_CHUVA >= LIMITE_CHUVA_ALTA);
  bool chuva_moderada = (PROBABILIDADE_CHUVA >= LIMITE_CHUVA_MODERADA &&
                         PROBABILIDADE_CHUVA < LIMITE_CHUVA_ALTA);

  // LÓGICA DE DECISÃO INTELIGENTE:
  // 1. DESLIGAR bomba SE: chuva alta (>= 70%) - ECONOMIA DE ÁGUA
  // 2. DESLIGAR bomba SE: solo encharcado OU condições não ideais
  // 3. AVALIAR SE: chuva moderada (>= 40%) - pode ou não ligar
  // 4. LIGAR bomba SE: solo seco E pH ideal E NPK OK E sem previsão de chuva

  bool deve_desligar = solo_encharcado || !ph_ok || !npk_ok || chuva_alta;
  bool deve_ligar = solo_seco && ph_ok && npk_ok && !chuva_alta && !chuva_moderada;

  // Exibir análise
  Serial.print("NPK Completo: ");
  Serial.println(npk_ok ? "SIM ✓" : "NÃO ✗");

  Serial.print("pH Adequado:  ");
  Serial.println(ph_ok ? "SIM ✓" : "NÃO ✗");

  Serial.print("Solo Seco:    ");
  Serial.println(solo_seco ? "SIM" : "NÃO");

  Serial.print("Solo Encharcado: ");
  Serial.println(solo_encharcado ? "SIM" : "NÃO");

  Serial.print("Previsão Chuva: ");
  Serial.print(PROBABILIDADE_CHUVA);
  Serial.print("% ");
  if (chuva_alta) {
    Serial.println("(ALTA - ECONOMIA)");
  } else if (chuva_moderada) {
    Serial.println("(MODERADA - CAUTELA)");
  } else {
    Serial.println("(BAIXA)");
  }

  Serial.println();

  // ========== CONTROLAR BOMBA COM BASE EM ANÁLISE INTELIGENTE ==========
  if (chuva_alta && bomba_ligada) {
    // Caso especial: Se houver previsão de chuva alta, desligar mesmo que solo esteja seco
    desligarBomba();
    Serial.println("💡 ECONOMIA: Chuva prevista, desligando irrigação");
  } else if (deve_ligar && !bomba_ligada) {
    ligarBomba();
  } else if (deve_desligar && bomba_ligada) {
    desligarBomba();
  } else if (chuva_moderada && solo_seco && bomba_ligada) {
    // Se já está ligada e há chuva moderada, manter mas avisar
    Serial.println("⚡ BOMBA: LIGADA (mas será desligada se chover)");
  } else if (bomba_ligada) {
    Serial.println("⚡ BOMBA: LIGADA (mantendo irrigação)");
  } else {
    Serial.println("⭕ BOMBA: DESLIGADA");
    if (chuva_moderada && solo_seco) {
      Serial.println("💡 ATENÇÃO: Solo seco mas há possibilidade de chuva");
    }
  }

  // Exibir recomendações se necessário
  exibirRecomendacoes(npk_ok, ph_ok, solo_seco, solo_encharcado, chuva_alta, chuva_moderada);
}

// ========== FUNÇÃO: LIGAR BOMBA ==========
void ligarBomba() {
  digitalWrite(PIN_RELE, HIGH);
  bomba_ligada = true;
  Serial.println("🚨 AÇÃO: LIGANDO BOMBA D'ÁGUA!");
  Serial.println("⚡ BOMBA: LIGADA (iniciando irrigação)");
}

// ========== FUNÇÃO: DESLIGAR BOMBA ==========
void desligarBomba() {
  digitalWrite(PIN_RELE, LOW);
  bomba_ligada = false;
  Serial.println("🚨 AÇÃO: DESLIGANDO BOMBA D'ÁGUA!");
  Serial.println("⭕ BOMBA: DESLIGADA");
}

// ========== FUNÇÃO: EXIBIR RECOMENDAÇÕES ==========
void exibirRecomendacoes(bool npk_ok, bool ph_ok, bool solo_seco, bool solo_encharcado,
                         bool chuva_alta, bool chuva_moderada) {
  bool tem_problema = !npk_ok || !ph_ok || solo_encharcado || chuva_alta;

  if (tem_problema || chuva_moderada) {
    Serial.println("\n⚠️  RECOMENDAÇÕES:");

    if (chuva_alta) {
      Serial.println("   🌧️  Chuva prevista - Irrigação desligada automaticamente");
      Serial.println("   💰 ECONOMIA DE ÁGUA estimada: 100-300 litros");
    }

    if (chuva_moderada && !chuva_alta) {
      Serial.println("   🌦️  Possibilidade de chuva - Monitorar clima");
      Serial.println("   💡 Se chover, sistema economizará água");
    }

    if (!npk_ok) {
      if (!nitrogenio_ok) Serial.println("   - Aplicar fertilizante nitrogenado");
      if (!fosforo_ok) Serial.println("   - Aplicar fertilizante fosfatado");
      if (!potassio_ok) Serial.println("   - Aplicar fertilizante potássico");
    }

    if (!ph_ok && !chuva_alta) {
      if (ph_solo < PH_MIN) {
        Serial.println("   - Solo muito ácido: aplicar calcário");
      } else {
        Serial.println("   - Solo muito alcalino: aplicar enxofre");
      }
    }

    if (solo_encharcado) {
      Serial.println("   - Melhorar drenagem do solo");
      Serial.println("   - Reduzir frequência de irrigação");
    }
  }
}
