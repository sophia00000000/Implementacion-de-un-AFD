# Implementacion-de-un-AFD

Implementación de un automata finito determinista en C y en Python.

Recibe dos archivos de txt.

- Config.txt = configuración del AFD

- Cadenas.txt = cadenas de prueba

---
### Formato archivos de entrada

- Config.txt
  
      # lista de estados
      Q = q0,q1,q2,q3
      # alfabeto
      Sigma = 0,1
      # Estado inicial
      q0 = q0
      # Estados de acceptación 
      F = q2
      # Matriz de transiciones: estado, símbolo = destino
      delta = 
      q0,0=q1
      q0,1=q0
      q1,0=q2
      q1,1=q3
      q2,0=q2
      q2,1=q1
      q3,0=q3
      q3,1=q2

- Cadenas.txt

        # cadenas de prueba
        001
        101
        0011
        1100
        1

---

### Casos de prueba

### 1.

Python:

<img width="727" height="350" alt="imagen" src="https://github.com/user-attachments/assets/af822653-01cd-4226-92a3-b91d19c5b578" />

C:

<img width="796" height="320" alt="imagen" src="https://github.com/user-attachments/assets/96a73608-df3e-4115-9d78-ac5e21723e72" />


### 2

<img width="1202" height="523" alt="imagen" src="https://github.com/user-attachments/assets/08878831-da03-4e0d-ad39-aa5730b2fd2f" />

Python:

<img width="711" height="405" alt="imagen" src="https://github.com/user-attachments/assets/339f6b33-4bbe-4503-945e-2a1cfb38d7e5" />

C:

<img width="796" height="414" alt="imagen" src="https://github.com/user-attachments/assets/e5ecbe0a-2554-4483-a139-698790b86a2c" />




### 3

<img width="1194" height="770" alt="imagen" src="https://github.com/user-attachments/assets/1f997fd3-a561-4692-a88d-7d9c98256d4b" />

Python:

<img width="704" height="436" alt="imagen" src="https://github.com/user-attachments/assets/b30c8503-a965-4404-8b76-3abcff4c6b25" />

C:

<img width="796" height="428" alt="imagen" src="https://github.com/user-attachments/assets/dac4c6e8-d046-497a-9051-a416c04883d6" />



### 4

Solo acpeta cadenas que inicien con a

<img width="222" height="184" alt="imagen" src="https://github.com/user-attachments/assets/07c5ee59-ae2e-4a0a-bb44-10427874fe37" />

Python:

<img width="795" height="363" alt="imagen" src="https://github.com/user-attachments/assets/3a2a4271-4fcb-4c4e-88b5-2ff8ccb4bdd0" />



C:

<img width="795" height="363" alt="imagen" src="https://github.com/user-attachments/assets/196fbebe-3caa-48df-bcc7-a0bdd368a5c8" />
