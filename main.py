# %%
from machine import SPI, Pin
import time

press_counts    : int = 0
pressure        : float = 0.0
percentage      : float = 0.0

outputmax       : int = 15099494
outputmin       : int = 1677722

pmax            : int = 1
pmin            : int = 0


SS = Pin(10, Pin.OUT)
SS.value(1)
spi=SPI(1, 200_000)
# spi=SPI(0, 100_000,polarity=1,phase=1)

cmd = bytearray([0xAA, 0x00, 0x00])

spi.write('test') 
spi.read(5) 
buf=bytearray(3)
spi.write_readinto('out',buf)
# %%
