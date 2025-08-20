import sys

# extraer coonfiguación
def parsear_config(archivo_config):
    estados = []
    alfabeto = []
    estado_inicial = ""
    estados_finales = []
    transiciones = {}
    
    with open(archivo_config, 'r', encoding='utf-8') as file:
        lineas = file.readlines()
    
    for linea in lineas:
        linea = linea.strip()
        if not linea or linea.startswith('#'):
            continue
            
        # Remover espacios alrededor del =
        if '=' in linea:
            partes = linea.split('=', 1)  # Dividir solo en el primer =
            izquierda = partes[0].strip()
            derecha = partes[1].strip()

            # Estados
            if izquierda.upper() == 'Q':
                estados = [q.strip() for q in derecha.split(',') if q.strip()]
            
            # Alfabeto
            elif izquierda.upper() == 'SIGMA':
                alfabeto = [s.strip() for s in derecha.split(',') if s.strip()]
            
            # Estado inicial
            elif izquierda.upper() == 'Q0':
                estado_inicial = derecha
            
            # Estados finales
            elif izquierda.upper() == 'F':
                estados_finales = [f.strip() for f in derecha.split(',') if f.strip()]

            # Transiciones
            elif '=' in linea and ',' in linea:
                #Parsear q0,0=q1
                parte_izq, destino = linea.split('=')
                estado, simbolo = parte_izq.split(',')
                transiciones[(estado.strip(), simbolo.strip())] = destino.strip()
    
    return estados, alfabeto, estado_inicial, estados_finales, transiciones

def leer_cadenas(archivo_cadenas):
    cadenas = []
    with open(archivo_cadenas, 'r', encoding='utf-8') as file:
        for linea in file:
            cadena = linea.strip()
            if cadena:  # Ignorar líneas vacías
                cadenas.append(cadena)
    return cadenas

def afd(cadena, estado_inicial, estados_finales, transiciones):
    estado_actual = estado_inicial
    for simbolo in cadena:
        # funcion de transición general δ(q,a) = q'
        if (estado_actual, simbolo) in transiciones:
            estado_actual = transiciones[(estado_actual, simbolo)]
        else:
            #Si no hay transición definida
            return False
    
    #aceptada si termina en un estado final
    return estado_actual in estados_finales

def main():
    # Archivos de entrada
    archivo_config = sys.argv[1]
    archivo_cadenas = sys.argv[2]

    #Parsear configuración del AFD
    estados, alfabeto, estado_inicial, estados_finales, transiciones = parsear_config(archivo_config)
    
    #información
    print(f"Estados: {estados}")
    print(f"Alfabeto: {alfabeto}")
    print(f"Estado inicial: {estado_inicial}")
    print(f"Estados finales: {estados_finales}")
    print(f"Transiciones: {transiciones}")
    
    #procesar cadenas de prueba
    cadenas = leer_cadenas(archivo_cadenas)
    
    print("Resultados:")
    for cadena in cadenas:
        resultado = afd(cadena, estado_inicial, estados_finales, transiciones)
        estado_resultado = "ACEPTA" if resultado else "NO ACEPTA"
        print(f"'{cadena}' -> {estado_resultado}")

if __name__ == "__main__":
    main()