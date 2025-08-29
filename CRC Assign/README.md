# Cyclic Redundancy Check (CRC) Simulation in C++

## 📌 Overview
This project is a simple **CRC (Cyclic Redundancy Check) simulation** implemented in C++.  
It demonstrates **error detection** during data transmission by applying CRC encoding and checking **per character** instead of processing the entire string at once.  

The program simulates both **Sender’s Side** and **Receiver’s Side** operations:
- At the **Sender’s Side**, each character of the input string is:
  - Converted into its ASCII and 7-bit binary form.
  - Appended with zeroes (equal to the degree of the generator polynomial).
  - Divided by the generator polynomial to obtain the **CRC remainder**.
  - Combined with the dataword to form the **codeword** to be transmitted.

- At the **Receiver’s Side**, the received codeword is:
  - Either transmitted **without error** OR with a **random error induced**.
  - Checked using the same generator polynomial to compute the **syndrome**.
  - If the syndrome is `0`, the data is accepted as error-free.
  - Otherwise, the program detects that **CRC has failed**.

---

## ⚙️ Features
- CRC calculation on **individual characters**.
- Supports user-defined **generator polynomial**.
- Shows detailed **step-by-step output**:
  - Character, ASCII value, binary value
  - CRC remainder
  - Codeword generation
  - Receiver error detection
- Simulates **random bit errors** at the receiver’s end.

---

## 🖥️ Sample Output

### Input
```
Enter generator polynomial (binary): 1101
Enter no. of zeroes to augment: 3
Enter string: AB
```

### Output
```
<Sender's Side>

Character: A
ASCII value: 65
Binary value: 1000001
CRC remainder: 011
Codeword to be sent = 1000001011

<Receiver's Side>
Do you want to induce error (y/n)? n

No error induced.
Received Codeword = 1000001011
Syndrome = 000
There is no error.
Fetch the dataword:
Binary no: 1000001
Alphabet: A

Character: B
ASCII value: 66
Binary value: 1000010
CRC remainder: 010
Codeword to be sent = 1000010010

<Receiver's Side>
Do you want to induce error (y/n)? y

Random error generated!
Received Codeword = 1000010011
Syndrome after error: 101
Therefore this is error → CRC failed.
```

---

## 📖 Explanation of Key Terms
- **Generator Polynomial**: Binary representation of the polynomial used for CRC division.  
  Example: `1101` represents \( x^3 + x^2 + 1 \).
- **Dataword**: The original binary representation of the character.
- **Remainder (CRC bits)**: The result of dividing the dataword (with augmented zeros) by the generator polynomial.
- **Codeword**: Dataword + CRC remainder (transmitted to receiver).
- **Syndrome**: Result of checking the received codeword. If `0`, no error is detected.

---

## 🚀 How to Run
1. Clone the repository:
   ```bash
   git clone https://github.com/<your-username>/crc-simulation.git
   ```
2. Navigate into the project directory:
   ```
   cd crc-simulation
   ```
3. Compile the program:
   ```
   g++ crc.cpp -o crc
   ```
4. Run the executable:
   ```
   ./crc
   ```
