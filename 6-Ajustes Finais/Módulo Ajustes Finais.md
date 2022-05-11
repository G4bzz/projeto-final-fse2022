# Ajustes finais

## Novos componentes:
![Novos componentes](../midia/componentes_buzzerVentilador.png "Novos componentes")

Como mecanismos para tratamento de possíveis falhas, foram acrescidos dois componentes: um buzzer (à direita) e um ventilador (simbolicamente representado por um LED).

## Informações:
O buzzer tem como finalidade de servir como um alarme que vai ficar disparando sinais sonoros até que a situação se normalize.

Ele pode ser acionado toda vez que a temperatura estiver abaixo de 35°C ou acima de 40°C, pois essas faixas de temperatura são prejudiciais para os pintinhos.
- Caso a temperatura esteja abaixo de 35, além do buzzer, as lâmpadas também serão acionadas para conseguirem estabalizar a temperatura, assim que a temperatura ideal for atingida, elas se apagam.
- Caso a temperatura supere os 40°C, além do buzzer, o ventilador para que a temperatura baixe e se normalize. Assim que o viveiro atingir a temperatura ideial, tanto o ventilador quanto o buzzer serão desligados.

## Montagem:
![Adição do buzzer e ventilador](../midia/esq_buzzerVentilador.png "Adição do buzzer e ventilador")

## Código:
Por estar muito extenso, o código não será colocado aqui. O código-fonte pode ser acessado no arquivo [ajustes.ino](./ajustes.ino)