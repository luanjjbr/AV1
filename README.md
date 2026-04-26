# AV1 - Sistema Discreto com ESP32 e Display OLED

## Sobre o projeto

Este projeto implementa, em **ESP32 + ESP-IDF**, um sistema discreto configuravel com interface por **serial** e visualizacao em **display OLED SSD1306**.

O firmware permite:

- selecionar diferentes modelos discretos, como `4`, `8`, `11`, `12`, `104`, `108`, `111` e `112`
- configurar os parametros `a` e `T` pela serial
- aplicar tipos de entrada como **impulso**, **degrau**, **rampa** e **reset**
- visualizar no OLED o estado atual do sistema
- imprimir no terminal serial a entrada e a saida calculadas

Em resumo, o projeto funciona como uma pequena plataforma de testes para modelos discretos, com interacao em tempo real e feedback visual no display.

## Tecnologias usadas

- **ESP32**
- **ESP-IDF v6.0**
- **FreeRTOS**
- **SSD1306 via I2C**
- **UART/Serial**
- **LEDC PWM**

Pelos arquivos do projeto, o ambiente atual esta configurado com:

- alvo: `esp32`
- porta serial: `COM4`
- ESP-IDF: `C:\esp\v6.0\esp-idf`

## Estrutura do projeto

O projeto foi organizado em componentes para separar responsabilidades:

- [main/main.c](C:/Users/italo/Desktop/Teste/Controle2/AV1/main/main.c:1)
  Responsavel pela inicializacao do sistema, criacao da task principal de calculo e bootstrap do projeto.

- [components/mz/mz.c](C:/Users/italo/Desktop/Teste/Controle2/AV1/components/mz/mz.c:1)
  Implementa a logica matematica do sistema discreto. Mantem historico de entrada e saida, configura os coeficientes dos modelos e calcula a resposta do sistema.

- [components/serial_monitor/serial_monitor.c](C:/Users/italo/Desktop/Teste/Controle2/AV1/components/serial_monitor/serial_monitor.c:1)
  Le comandos da serial, interpreta entradas do usuario e atualiza os parametros/modelos em tempo real.

- [components/menu_display/menu_display.c](C:/Users/italo/Desktop/Teste/Controle2/AV1/components/menu_display/menu_display.c:1)
  Controla a tela do OLED, exibindo `a`, frequencia/tempo de amostragem e o modelo atual.

- [components/ssd1306/ssd1306.c](C:/Users/italo/Desktop/Teste/Controle2/AV1/components/ssd1306/ssd1306.c:1)
  Driver do display OLED SSD1306 via I2C.

- [components/Button/Button.c](C:/Users/italo/Desktop/Teste/Controle2/AV1/components/Button/Button.c:1)
  Le o botao fisico.

- [components/led/led.c](C:/Users/italo/Desktop/Teste/Controle2/AV1/components/led/led.c:1)
  Controle do LED.

- [components/pwm_driver/pwm_driver.c](C:/Users/italo/Desktop/Teste/Controle2/AV1/components/pwm_driver/pwm_driver.c:1)
  Inicializacao e controle de PWM.

## Como o projeto funciona

O fluxo principal e este:

1. O `app_main()` inicializa LED, botao, PWM e display.
2. O sistema seleciona um modelo inicial padrao.
3. A task principal executa periodicamente o calculo do sistema discreto.
4. A task da serial fica aguardando comandos do usuario.
5. Quando o usuario muda modelo ou parametros, o calculo interno e o menu do OLED sao atualizados.

O calculo do sistema usa:

- `u[]` para historico da entrada
- `y[]` para historico da saida
- `num[]` e `den[]` como coeficientes do modelo atual

Cada item configurado em `config_item()` monta um conjunto diferente de coeficientes para representar uma forma discreta especifica.

## O que aparece no display

O OLED mostra um menu simples com quatro linhas:

- identificacao do projeto
- valor atual de `a`
- frequencia e periodo de amostragem
- modelo atual

Exemplo:

```text
IFCE: Luan
a=1.5
10Hz|0.100s
Modelo: 4
```

## Como usar

### 1. Compilar

No terminal do ESP-IDF:

```powershell
idf.py build
```

### 2. Gravar no ESP32

```powershell
idf.py -p COM4 flash
```

### 3. Abrir o monitor serial

```powershell
idf.py -p COM4 monitor
```

## Comandos da serial

Os comandos aceitos atualmente sao:

- `i`
  Aplica impulso.

- `d`
  Aplica degrau.

- `r`
  Ativa rampa.

- `R`
  Reseta o estado interno.

- `g`
  Tambem executa reset.

- `p`
  Alterna a flag de print.

- `4`, `8`, `11`, `12`, `104`, `108`, `111`, `112`
  Selecionam o modelo discreto.

- `a,T`
  Define os parametros do sistema.

Exemplo:

```text
1.5,0.1
```

Isso significa:

- `a = 1.5`
- `T = 0.1 s`

Depois da configuracao, o menu do OLED e atualizado automaticamente.

## Como foi feito

O projeto foi construido com uma abordagem modular:

- a **logica de calculo** ficou isolada no componente `mz`
- a **interacao serial** ficou separada no componente `serial_monitor`
- a **interface visual** foi centralizada no componente `menu_display`
- os drivers de hardware ficaram separados em componentes menores

Essa divisao facilita:

- manutencao
- testes por responsabilidade
- evolucao do projeto
- leitura do codigo

Tambem houve uma preocupacao em deixar o `main` mais simples, usando-o como ponto de orquestracao e nao como lugar para concentrar toda a regra de negocio.

## Saidas observadas

Durante a execucao, a serial imprime informacoes como:

```text
>Entrada:...
>Saida_Y:...
```

Isso permite acompanhar a resposta do modelo em tempo real.

## Possiveis melhorias

Algumas melhorias naturais para o projeto sao:

- documentar matematicamente cada modelo (`4`, `8`, `11`, `12`, etc.)
- criar um menu mais completo no display
- ligar a flag de print a uma logica real de habilitar/desabilitar logs
- adicionar tratamento mais robusto para falhas do OLED
- transformar a task de calculo em componente proprio
- adicionar testes ou simulacoes offline para validar os coeficientes de cada modelo
- criar uma tabela de comandos e exemplos de entrada/saida para uso em laboratorio

## Observacoes

- O projeto depende de uma configuracao correta do ambiente ESP-IDF.
- Se houver erro de build, normalmente vale conferir `IDF_PATH`, Python do ESP-IDF e a porta serial usada.
- O display e a serial sao partes importantes do fluxo, entao testar ambos ajuda bastante a validar o sistema completo.

## Autor e contexto

Pelo conteudo mostrado no display e pelos comentarios do codigo, o projeto parece ter sido montado como apoio a atividades de estudo, laboratorio ou demonstracao de sistemas discretos no contexto academico.

---

Se quiser, no proximo passo eu posso transformar este README em uma versao ainda mais completa, com:

- diagrama da arquitetura
- tabela formatada de comandos
- passo a passo de hardware
- exemplos de uso para cada modelo
