import machine,time,math # Importar librerías
from time import sleep_ms, ticks_ms
from machine import I2C, Pin
from esp8266_i2c_lcd import I2cLcd

i2c = I2C(scl=Pin(5), sda=Pin(16), freq=400000)
lcd = I2cLcd(i2c,0x3F,2,16)

pinA = machine.Pin(0, machine.Pin.IN, machine.Pin.PULL_UP) # Pin de entrada delbotón 1
pinB = machine.Pin(2, machine.Pin.IN, machine.Pin.PULL_UP) # Pin de entrada delbotón 2
pinC = machine.Pin(12, machine.Pin.IN, machine.Pin.PULL_UP) # Pin de entrada delbotón 3
pinD = machine.Pin(14, machine.Pin.IN, machine.Pin.PULL_UP) # Pin de entrada delbotón 4

pinX = machine.Pin(13, machine.Pin.OUT) # Pin de control conectado al pin INA delVNH2SP30
pinY = machine.Pin(15, machine.Pin.OUT) # Pin de control conectado al pin INB delVNH2SP30
a = 20.# tau inicial
l = 0. # valor abs
b = 0. # Amplitud
t = 0. # tiempo
x = 0.# duty cicle del pwm

while True: # Ciclo infinito
	Am = b #Amplitud
	tau = a #Periodo
	if tau == 0:
            tau = 0+2
        f = 1./tau#frecuencia
        w = 2.*math.pi*f #frecuencia angular
        dt = tau/100.#tiempo de muestras sobre tau
        y = Am*(math.sin(w*t))
        l = (abs(y)) # valor absoluto de duty
        x = math.floor(l) #convertir a entero
        PWM = machine.Pin(4)
        PWM_pwm = machine.PWM(PWM)
        PWM_pwm.freq(1000)
        PWM_pwm.duty(x)
        print('onda',y,'t',t,'p',math.fmod(t,tau), 'O_A',l,'pwm', x , 'Am', Am)
        time.sleep(dt)#tiempo de espera de muestreo
        t = t+dt#tiempo mas tiempo de muestro
        
        lcd.clear()
        z = Am
        lcd.move_to(0,0)
        zz = str(z)
        lcd.putstr('Amplitud = ' + zz)
        w = tau
        ww = str(w)
        lcd.move_to(0,1)
        lcd.putstr('Periodo = '+ ww)
	if y > 0: # Si el valor es mayor que cero enviar las salidas siguientes por lo pines de control
        	pinX.value(1)
        	pinY.value(0)
	if y < 0: # Si el valor es menor que cero enviar las salidas siguientes por lo pines de control
        	pinX.value(0)
        	pinY.value(1)
	if pinB.value () ==0: # Si se oprime el botón B
        	b = Am + 10 # Incrementar "AMPLITUD" en 10
        	t = 0
        time.sleep(.01) # Retardo para el rebote de corriente del pulsador
	if pinA.value () ==0: # Si se oprime el botón A
        	b = Am - 10 # Decrementar "AMPLITUD" en 10
        	t = 0
        print ('amplitud',Am) # Imprimir la variable "amplitud"
        time.sleep(.01) # Retardo
	if pinC.value () ==0:#Si se oprime el botón C
        	a = tau + 2 # Incrementar "period0"
        	t = 0
        time.sleep(.01) # Retardo para el rebote de corriente del pulsador
	if pinD.value () ==0: # Si se oprime el botón D
        	a = tau - 2 # Decrementar "period0" en 2
        	t = 0
        print ('periodo',tau) # Imprimir la variable "periodo"
        time.sleep(.01) # RetardoDT
