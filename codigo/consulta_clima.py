#!/usr/bin/env python3
"""
FARMTECH SOLUTIONS - Consulta de Dados Meteorológicos
Autor: Jefferson de Souza Oliveira - RM568138
Curso: Inteligência Artificial - FIAP Fase 2

DESCRIÇÃO:
Script Python que consulta API meteorológica pública (wttr.in)
e salva os resultados em JSON para uso no sistema de irrigação ESP32.

INTEGRAÇÃO COM ESP32:
Os dados salvos em 'clima_atual.json' podem ser usados manualmente
no código do ESP32 para otimizar decisões de irrigação baseadas
em previsão climática.

API UTILIZADA:
- wttr.in (gratuita, sem necessidade de chave)
- Formato JSON completo
"""

import json
import urllib.request
import urllib.error
from datetime import datetime

def obter_clima(cidade="Sao Paulo"):
    """
    Obtém dados meteorológicos da API wttr.in

    Args:
        cidade (str): Nome da cidade para consulta

    Returns:
        dict: Dicionário com dados climáticos ou None em caso de erro
    """
    print(f"[INFO] Consultando API meteorologica para {cidade}...")

    # URL da API wttr.in em formato JSON
    cidade_encoded = cidade.replace(" ", "%20")
    url = f"https://wttr.in/{cidade_encoded}?format=j1"

    try:
        # Fazer requisição HTTP
        with urllib.request.urlopen(url, timeout=10) as response:
            data = json.loads(response.read().decode())

        # Extrair dados relevantes
        current = data['current_condition'][0]
        weather_today = data['weather'][0]
        hourly = weather_today['hourly'][0]

        # Estruturar dados para irrigação
        clima_info = {
            'cidade': cidade,
            'timestamp': datetime.now().strftime('%Y-%m-%d %H:%M:%S'),
            'fonte': 'wttr.in API',

            # Condições atuais
            'temperatura_atual': int(current['temp_C']),
            'umidade_relativa': int(current['humidity']),
            'condicao': current['weatherDesc'][0]['value'],
            'precipitacao_mm': float(current.get('precipMM', 0)),

            # Previsão
            'probabilidade_chuva': int(hourly['chanceofrain']),
            'temp_maxima': int(weather_today['maxtempC']),
            'temp_minima': int(weather_today['mintempC']),
            'vento_kmh': int(current['windspeedKmph']),

            # Recomendação para irrigação
            'recomendacao_irrigacao': None  # Será calculado abaixo
        }

        # Calcular recomendação de irrigação
        prob_chuva = clima_info['probabilidade_chuva']
        precipitacao = clima_info['precipitacao_mm']

        if prob_chuva >= 70 or precipitacao > 5.0:
            clima_info['recomendacao_irrigacao'] = "DESLIGAR"
            clima_info['motivo'] = f"Alta probabilidade de chuva ({prob_chuva}%)"
        elif prob_chuva >= 40:
            clima_info['recomendacao_irrigacao'] = "REDUZIR"
            clima_info['motivo'] = f"Probabilidade moderada de chuva ({prob_chuva}%)"
        else:
            clima_info['recomendacao_irrigacao'] = "NORMAL"
            clima_info['motivo'] = f"Baixa probabilidade de chuva ({prob_chuva}%)"

        print("[OK] Dados obtidos com sucesso!")
        return clima_info

    except urllib.error.URLError as e:
        print(f"[ERRO] Erro de conexao: {e.reason}")
        return None
    except json.JSONDecodeError:
        print("[ERRO] Erro ao processar resposta da API")
        return None
    except KeyError as e:
        print(f"[ERRO] Erro ao extrair dados: campo {e} nao encontrado")
        return None
    except Exception as e:
        print(f"[ERRO] Erro inesperado: {str(e)}")
        return None


def gerar_dados_backup(cidade):
    """
    Gera dados simulados caso a API não esteja acessível

    Args:
        cidade (str): Nome da cidade

    Returns:
        dict: Dicionário com dados simulados
    """
    import random

    print("[AVISO] Usando dados simulados (backup)")

    prob_chuva = random.randint(0, 100)

    clima_info = {
        'cidade': cidade,
        'timestamp': datetime.now().strftime('%Y-%m-%d %H:%M:%S'),
        'fonte': 'dados simulados (backup)',

        'temperatura_atual': random.randint(18, 28),
        'umidade_relativa': random.randint(50, 90),
        'condicao': random.choice(['Ensolarado', 'Parcialmente nublado', 'Nublado', 'Chuvoso']),
        'precipitacao_mm': round(random.uniform(0, 10), 1),

        'probabilidade_chuva': prob_chuva,
        'temp_maxima': random.randint(22, 32),
        'temp_minima': random.randint(15, 20),
        'vento_kmh': random.randint(5, 30),

        'recomendacao_irrigacao': None
    }

    # Calcular recomendação
    if prob_chuva >= 70:
        clima_info['recomendacao_irrigacao'] = "DESLIGAR"
        clima_info['motivo'] = f"Alta probabilidade de chuva ({prob_chuva}%)"
    elif prob_chuva >= 40:
        clima_info['recomendacao_irrigacao'] = "REDUZIR"
        clima_info['motivo'] = f"Probabilidade moderada de chuva ({prob_chuva}%)"
    else:
        clima_info['recomendacao_irrigacao'] = "NORMAL"
        clima_info['motivo'] = f"Baixa probabilidade de chuva ({prob_chuva}%)"

    return clima_info


def salvar_json(dados, arquivo='clima_atual.json'):
    """
    Salva dados climáticos em arquivo JSON

    Args:
        dados (dict): Dados climáticos
        arquivo (str): Nome do arquivo de saída

    Returns:
        bool: True se salvou com sucesso, False caso contrário
    """
    try:
        with open(arquivo, 'w', encoding='utf-8') as f:
            json.dump(dados, f, ensure_ascii=False, indent=2)
        print(f"[OK] Dados salvos em '{arquivo}'")
        return True
    except Exception as e:
        print(f"[ERRO] Erro ao salvar arquivo: {str(e)}")
        return False


def exibir_resumo(dados):
    """
    Exibe resumo formatado dos dados climáticos

    Args:
        dados (dict): Dados climáticos
    """
    print("\n" + "="*60)
    print("RESUMO DOS DADOS METEOROLOGICOS")
    print("="*60)
    print(f"Cidade: {dados['cidade']}")
    print(f"Atualizado: {dados['timestamp']}")
    print(f"Fonte: {dados['fonte']}")
    print()
    print(f"Temperatura: {dados['temperatura_atual']}C (Min: {dados['temp_minima']}C | Max: {dados['temp_maxima']}C)")
    print(f"Umidade Relativa: {dados['umidade_relativa']}%")
    print(f"Condicao: {dados['condicao']}")
    print(f"Precipitacao: {dados['precipitacao_mm']} mm")
    print(f"Probabilidade de Chuva: {dados['probabilidade_chuva']}%")
    print(f"Vento: {dados['vento_kmh']} km/h")
    print()
    print("="*60)
    print("RECOMENDACAO PARA IRRIGACAO")
    print("="*60)

    recomendacao = dados['recomendacao_irrigacao']
    motivo = dados['motivo']

    if recomendacao == "DESLIGAR":
        print(f"[X] {recomendacao}: {motivo}")
        print("   Sugestao: Desligar irrigacao para economizar agua")
    elif recomendacao == "REDUZIR":
        print(f"[!] {recomendacao}: {motivo}")
        print("   Sugestao: Reduzir tempo de irrigacao em 50%")
    else:
        print(f"[OK] {recomendacao}: {motivo}")
        print("   Sugestao: Manter irrigacao conforme sensores")

    print("="*60)


def gerar_codigo_arduino(dados):
    """
    Gera snippet de código C++ para copiar no Arduino

    Args:
        dados (dict): Dados climáticos
    """
    print("\n" + "="*60)
    print("CODIGO PARA COPIAR NO ESP32 (Arduino)")
    print("="*60)
    print("// Dados meteorológicos obtidos via Python API")
    print(f"// Última atualização: {dados['timestamp']}")
    print(f"// Fonte: {dados['fonte']}")
    print()
    print(f"#define PROBABILIDADE_CHUVA {dados['probabilidade_chuva']}  // %")
    print(f"#define PRECIPITACAO_MM {dados['precipitacao_mm']:.1f}      // mm")
    print(f"#define TEMP_ATUAL {dados['temperatura_atual']}            // °C")
    print(f"#define UMIDADE_AR {dados['umidade_relativa']}            // %")
    print()

    if dados['recomendacao_irrigacao'] == "DESLIGAR":
        print("// RECOMENDAÇÃO: Desligar irrigação (chuva prevista)")
        print("bool previsao_chuva = true;")
    elif dados['recomendacao_irrigacao'] == "REDUZIR":
        print("// RECOMENDAÇÃO: Reduzir irrigação (possível chuva)")
        print("bool previsao_chuva_moderada = true;")
    else:
        print("// RECOMENDAÇÃO: Irrigação normal")
        print("bool previsao_chuva = false;")

    print("="*60)


def main():
    """
    Função principal
    """
    print("="*60)
    print("FARMTECH SOLUTIONS - CONSULTA METEOROLOGICA")
    print("="*60)
    print()

    # Configurações
    CIDADE = "Sao Paulo"  # Maior cidade produtora de café: Minas Gerais
    ARQUIVO_JSON = "clima_atual.json"

    # Tentar obter dados da API
    dados = obter_clima(CIDADE)

    # Se falhar, usar dados simulados
    if dados is None:
        dados = gerar_dados_backup(CIDADE)

    # Exibir resumo
    exibir_resumo(dados)

    # Salvar em JSON
    if salvar_json(dados, ARQUIVO_JSON):
        print(f"[OK] Arquivo '{ARQUIVO_JSON}' criado com sucesso!")

    # Gerar código Arduino
    gerar_codigo_arduino(dados)

    print("\n[OK] Processo concluido!")
    print("Copie as constantes acima para o codigo ESP32")


if __name__ == "__main__":
    main()
