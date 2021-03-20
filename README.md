# Laboratorio Modular de Transferencia de Calor
El  **Laboratorio Modular de Transferencia de Calor** (LMTC) consiste originalmente en tres dispositivos modulares para realizar diversas prácticas en el laboratorio de la materia **Transferencia de calor** o su equivalente. Se ha agregado un dispositivo más que es económico y puede ser armado por cualquier persona con conocimientos básicos de electrónica y programación.

También es parte de este LMTC:
+ Guía de prácticas del LMTC
+ Guía de uso de los dispositivos
+ Este repositorio con el código y lo necesario para duplicar este LMTC


La idea principal del LMTC es un laboratorio formado por dispositivos modulares, donde cada dispositivo puede ser
usado de manera independiente o de manera conjunta con el resto de ellos, y se pueden agregar más dispositivos
del mismo tipo o excluirlos para el diseño de un experimento.

Los dispositivos están documentados [aquí](https://github.com/AltamarMx/LabModularCalor/tree/main/dispositivos) y son:

1. [**_Dispositivo de adquisición y visualización de datos_**:](https://github.com/AltamarMx/LabModularCalor/blob/main/dispositivos/d1.md) Es una RaspBerry Pi con una plataforma IoT instalada y configurada como servidor. Es útil en caso de no contar con internet y una conexión inalámbrica, se puede utilizar este dispositivo para que sirva de adquisición y visualización de datos en tiempo real. Si se cuenta con internet, se puede solicitar la alta del dispositivo al correo gbv@ier.unam.mx para que se proporcione una liga a un tablero de visualización de datos.

2.  [**_Dispositivo para medir temperatura y flujo de calor_**:](https://github.com/AltamarMx/LabModularCalor/blob/main/dispositivos/d2.md) Cuenta con 4 termopares tipo T y un sensor de flujo de calor que envia los datos medidos a una plataforma de IoT que puede ser el dispositivo anterior o una plataforma puesta en línea para este laboratorio.

3. [**_Dispositivo para generar condiciones de frontera_**:](https://github.com/AltamarMx/LabModularCalor/blob/main/dispositivos/d3.md)  Consiste en un microcontrolador con una celda Peltier que puede producir una condición de temperatura constante en el tiempo o que varie de forma sinusoidal.

4. [**_Dispositivo simplificado para medir temperatura_**:](https://github.com/AltamarMx/LabModularCalor/blob/main/dispositivos/d4.md)  Es el dispositivo más fácil y barato de recrear,
cuenta con 4 sensores digitales de temperatura pero se pueden agregar más. Solo requiere configurar la red inalámbrica a la que se debe conectar y energizarse por medio de un cable micro USB por medio de una computadora, batería o cargador de celular.

5. [**_Plataforma IoT:_**](https://github.com/AltamarMx/LabModularCalor/blob/main/dispositivos/plataforma.md) Plataforma IoT disponible previa solicitud, destinado al LMTC. En esta plataforma se pueden dar de alta los dispositivos para que en caso de contar con una conexión a internet, la adquisición y visualización de datos sea más sencilla y se pueda prescindir del primer dispositivo.






Este repositorio contiene toda la información relacionada para usar, replicar o mejorar el  **Laboratorio Modular de Transferencia de Calor** (LMTC) desarrollado en el Instituto de Energías Renovables con el apoyo del proyecto PAPIME  **PE112318**.
