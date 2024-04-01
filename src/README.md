Node MCU Clock
==============

**NodeMCU based clock with multiple types of chimes**

Up to 4 actuators are coded here, but more could be added.

Output pins from the nodeMCU board are connected to a Darlington Array IC with the following map:

| Name  | Node MCU | Arduino |
| ----- | -------- | ------- |
|  A    |    D3    |    5    |
|  B    |    D2    |    4    |
|  C    |    D1    |    0    |
|  D    |    D5    |   14    |
|  E&ast;   |    D6    |   12    |
|  F&ast;   |    D7    |   13    |
|  G&ast;   |    D8    |   15    |
&ast; not used here

**Defined chime types are:**

Hour Only (1 or 2 chimes)

Hour + Half (1 or 2 chimes)

Hour + Half (3 chimes)

All Quarters (2 chimes)

All Quarters (3 chimes)

Westminster (4 chimes)

Ship's (1 chime)

**A, B, C, D chimes are used as:**

1 Chime: A only

2 Chimes: A and B

3 Chimes: A, B, and C

4 Chimes: A, B, C, and D

**Chime types ring as follows:**

Hour Only - Only rings count for hour

Hour + Half - Rings once for half hour, count for hour

All Quarters - Rings set of chimes for quarters, plus count for hour

Westminster - All quarters using familiar Westminster tune

Ship's - Rings half hours and hours in 4 hour watches

Chime sequences are set in Chime.cpp
