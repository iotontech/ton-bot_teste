/**
******************************************************************************
* @file    main.cpp
* @author  Kleber Lima da Silva (kleber@ioton.cc)
* @version V0.0.1
* @date    19-Junho-2017
* @brief   Programa para testes do hardware do Robô TON-BOT.
******************************************************************************
* @attention
*
* COPYRIGHT(c) 2017 IOTON Technology
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup Teste TON-BOT
 * @{
 */

/* Variáveis privadas --------------------------------------------------------*/
Ticker imuTick;   /* Timer para o algoritmo da IMU */
Timer t1;         /* Timer auxiliar */


/* Funções Privadas ----------------------------------------------------------*/

/**
 * @brief  Callback para execução do Algoritmo AHRS com taxa de 10 Hz
 * @note   O algoritmo AHRS calcula os ângulos: roll, pitch and yaw
 * @param  Nenhum
 * @retval Nenhum
 */
void imuCallback(void)
{
  imu.runAHRS(0.1);
} // fim da imuCallback

/**
 * @brief  Programa Principal
 * @param  Nenhum
 * @retval Nenhum
 */
int main(void)
{
  float lf = 0, l = 0, r = 0, rf = 0;

  /* Ligue o TON com o botão USER pressionado para indicar o estado da bateria */
  if (ton.USERisPressed())
  {
    /* Necessário pressionar o botão RESET voltar ao modo normal */
    ton.batteryStatus();
  }

  /* Inicialização dos periféricos */
  ton.setLED(BLUE);
  initTonBot();
  ton.setLED(RED);
  ton.enableBluetooth();
  ton.enableIMU();  /* Deixe o robô parado durante a inicialização (~5s) */

  /* Configura o Timer da IMU - intervalo = 0.1 segundos */
  imuTick.attach(&imuCallback, 0.1);

  /* Inicio do programa ------------------------------------------------------*/
  usb.printf("Programa TESTE - TON-BOT!\r\n");
  ton.setLED(WHITE);
  wait(1);
  ton.setLED(NONE);
  beeps(3, 100, 50);

  resetEncoderEsquerda();
  resetEncoderDireita();

  /* LOOP principal ----------------------------------------------------------*/
  while (1)
  {
    /* Zera e inicia o timer para estabelecer a base de tempo */
    t1.reset();
    t1.start();

    /* Envia mensagens de estados dos periféricos a cada 1s para a USB */
    usb.printf("Sensores de Linha (erro): %d\r\n", getSensoresLinha());
    usb.printf("\r\n");

    usb.printf("Sensores de Paredes: %d\r\n", getSensoresParede(&lf, &l, &r, &rf));
    usb.printf("%0.3f | %0.3f | %0.3f | %0.3f\r\n", lf, l, r, rf);
    usb.printf("\r\n");

    usb.printf("IMU - Algoritmo AHRS:\r\n");
    usb.printf("pitch: %0.3f\r\n", imu.getPitch());
    usb.printf("roll: %0.3f\r\n", imu.getRoll());
    usb.printf("yaw: %0.3f\r\n", imu.getYaw());
    usb.printf("\r\n");

    usb.printf("Bateria: %0.3fV\r\n", ton.getBattery());
    usb.printf("\r\n");

    usb.printf("Encoders: %d | %d\r\n", getEncoderEsquerda(), getEncoderDireita());
    usb.printf("\r\n");

    usb.printf("---\r\n");


    /* Caso queira direcionar alguma mensagem para o Bluetooth, basta..
     * mudar de 'usb.' para 'bluetooth.' - como o exemplo a seguir: */
    bluetooth.printf("%0.3f | %0.3f | %0.3f | %0.3f\r\n", lf, l, r, rf);


    /* Se o botão for pressionado: liga os motores por 1 segundo
     * .. ou aguarda 1 segundo sem fazer nada */
    while (t1.read_ms() < 1000)
    {
      if (ton.USERisPressed())
      {
        ton.setLED(YELLOW);
        beep(100);
        setMotores(0.2, -0.2); /* CUIDADO COM VELOCIDADES ALTAS (~ 2.0 m/s) */
        wait(1);
        setMotores(0, 0);
        ton.setLED(NONE);
      }
    }


    /* Pisca o LED verde */
    ton.toggleLED(GREEN);

  } // fim do loop principal
} // fim da função principal


/**
 * @}
 */

/************************ (C) COPYRIGHT IOTON Technology **********************/
/***********************************END OF FILE********************************/
