# BV
Implementation of Succinct Bit Vector BV 

## Implemented features so far:
- PackedArray
Use an array of 64bit unsigned integer as an array of arbitrary length integer (shorter or equal than 64bit) -- necessary for succinct representation
- BV (on work)
Implemented rank index for rank(i)= (the number of 1s in B[0:i])