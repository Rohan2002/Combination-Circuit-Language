# Combination-Circuit-Language

# Example Query 1

```
INPUT 20   IN1 IN2 IN3 IN4 IN5 IN6 IN7 IN8 IN9 IN10 IN11 IN12 IN13 IN14 IN15 IN16 IN17 IN18 IN19 IN20
OUTPUT 1   OUT1

MULTIPLEXER 4   0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1   IN1 IN2 IN3 IN4           temp1
MULTIPLEXER 4   0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1   IN5 IN6 IN7 IN8           temp2
MULTIPLEXER 4   0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1   IN9 IN10 IN11 IN12        temp3
MULTIPLEXER 4   0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1   IN13 IN14 IN15 IN16       temp4
MULTIPLEXER 4   0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1   IN17 IN18 IN19 IN20       temp5
MULTIPLEXER 4   0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1   temp1 temp2 temp3 temp4   temp6
MULTIPLEXER 2   0 1 0 1                           temp5 temp6               OUT1
```

Query 1 will be read from top to bottom. All Input(s), Output(s), and Circuit Instructions and variables will be read, parsed, and applied to return a truth table. 
For Query 1 particularly, this language will generate 20 input bit combination, pass it through the Multiplexer circuit instructions, and generate 1 output. Therefore there will be 2^20 possible combinations of the input bits, and thus there will be 2^20 possible types of outputs generated from this particular query. 

# Project Credits

All code is written by Rohan Deshpande.
