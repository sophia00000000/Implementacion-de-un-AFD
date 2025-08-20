#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ESTADOS 100
#define MAX_ALFABETO 50
#define MAX_TRANSICIONES 500
#define MAX_CADENAS 100
#define MAX_LINEA 1000
#define MAX_NOMBRE_ESTADO 50

//Transición
typedef struct {
    char estado_origen[MAX_NOMBRE_ESTADO];
    char simbolo;
    char estado_destino[MAX_NOMBRE_ESTADO];
} Transicion;

//AFD
typedef struct {
    char estados[MAX_ESTADOS][MAX_NOMBRE_ESTADO];
    int num_estados;
    
    char alfabeto[MAX_ALFABETO];
    int tam_alfabeto;
    
    char estado_inicial[MAX_NOMBRE_ESTADO];
    
    char estados_finales[MAX_ESTADOS][MAX_NOMBRE_ESTADO];
    int num_estados_finales;
    
    Transicion transiciones[MAX_TRANSICIONES];
    int num_transiciones;
} AFD;

// quitar \n y espacios
void limpiar_cadena(char *str) {
    int len = strlen(str);
    while (len > 0 && (str[len-1] == '\n' || str[len-1] == '\r' || str[len-1] == ' ')) {
        str[len-1] = '\0';
        len--;
    }
}

//Encontrar '=' y obtener la parte después, ignorando espacios
char* obtener_valor_despues_igual(char *linea) {
    char *igual = strchr(linea, '=');
    if (igual == NULL) return NULL;
    
    igual++; // Avanzar después del '='
    // Saltar espacios en blanco después del '='
    while (*igual == ' ' || *igual == '\t') igual++;
    
    return igual;
}

//verificar si una línea empieza con una clave (ignorando espacios alrededor de '=')
int empieza_con_clave(char *linea, const char *clave) {
    int len_clave = strlen(clave);
    if (strncmp(linea, clave, len_clave) != 0) return 0;
    
    // Verificar que después de la clave viene '=' (con posibles espacios)
    char *resto = linea + len_clave;
    while (*resto == ' ' || *resto == '\t') resto++; // Saltar espacios
    
    return (*resto == '=');
}

// Función para dividir una cadena por comas
void dividir_por_comas(char *linea, char resultado[][MAX_NOMBRE_ESTADO], int *contador) {
    *contador = 0;
    char *token = strtok(linea, ",");
    
    while (token != NULL && *contador < MAX_ESTADOS) {
        // Quitar espacios al inicio
        while (*token == ' ') token++;
        strcpy(resultado[*contador], token);
        // Quitar espacios al final
        limpiar_cadena(resultado[*contador]);
        (*contador)++;
        token = strtok(NULL, ",");
    }
}

// Parsear archivo de configuración
void parsear_config(const char *archivo, AFD *afd) {
    FILE *file = fopen(archivo, "r");
    char linea[MAX_LINEA];
    
    afd->num_estados = 0;
    afd->tam_alfabeto = 0;
    afd->num_estados_finales = 0;
    afd->num_transiciones = 0;
    
    while (fgets(linea, sizeof(linea), file)) {
        limpiar_cadena(linea);
        
        // Ignorar líneas vacías, comentarios o línea "delta="
        if (strlen(linea) == 0 || linea[0] == '#' || strcmp(linea, "delta =") == 0 || strcmp(linea, "delta=") == 0) {
            continue;
        }
        
        if (empieza_con_clave(linea, "Q")) {
            // Parsear estados: Q=q0,q1,q2 o Q = q0,q1,q2
            char *valor = obtener_valor_despues_igual(linea);
            if (valor && strlen(valor) > 0) {
                char temp[MAX_LINEA];
                strcpy(temp, valor);
                dividir_por_comas(temp, afd->estados, &afd->num_estados);
            }
            
        } else if (empieza_con_clave(linea, "Sigma")) {
            // Parsear alfabeto: Sigma=0,1 o Sigma = 0,1
            char *valor = obtener_valor_despues_igual(linea);
            if (valor && strlen(valor) > 0) {
                char temp[MAX_LINEA];
                strcpy(temp, valor);
                char *token = strtok(temp, ",");
                afd->tam_alfabeto = 0;
                
                while (token != NULL && afd->tam_alfabeto < MAX_ALFABETO) {
                    while (*token == ' ') token++; // Quitar espacios
                    afd->alfabeto[afd->tam_alfabeto] = token[0];
                    afd->tam_alfabeto++;
                    token = strtok(NULL, ",");
                }
            }
            
        } else if (empieza_con_clave(linea, "q0")) {
            // Estado inicial: q0=q0 o q0 = q0
            char *valor = obtener_valor_despues_igual(linea);
            if (valor && strlen(valor) > 0) {
                strcpy(afd->estado_inicial, valor);
            }
            
        } else if (empieza_con_clave(linea, "F")) {
            // Estados finales: F=q2 o F = q2,q3
            char *valor = obtener_valor_despues_igual(linea);
            if (valor && strlen(valor) > 0) {
                char temp[MAX_LINEA];
                strcpy(temp, valor);
                dividir_por_comas(temp, afd->estados_finales, &afd->num_estados_finales);
            }
            
        } else if (strchr(linea, '=') && strchr(linea, ',')) {
            // Parsear transición: q0,0=q1 (incluso sin espacios)
            char temp_linea[MAX_LINEA];
            strcpy(temp_linea, linea);
            
            char *igual = strchr(temp_linea, '=');
            *igual = '\0';
            
            char *coma = strchr(temp_linea, ',');
            *coma = '\0';
            
            // Limpiar espacios de cada componente
            char *estado_origen = temp_linea;
            while (*estado_origen == ' ') estado_origen++;
            
            char *simbolo_str = coma + 1;
            while (*simbolo_str == ' ') simbolo_str++;
            
            char *estado_destino = igual + 1;
            while (*estado_destino == ' ') estado_destino++;
            
            strcpy(afd->transiciones[afd->num_transiciones].estado_origen, estado_origen);
            afd->transiciones[afd->num_transiciones].simbolo = simbolo_str[0];
            strcpy(afd->transiciones[afd->num_transiciones].estado_destino, estado_destino);
            
            afd->num_transiciones++;
        }
    }
    
    fclose(file);
}

// Buscar transición δ(q,a) = q'
char* buscar_transicion(AFD *afd, const char *estado_actual, char simbolo) {
    for (int i = 0; i < afd->num_transiciones; i++) {
        if (strcmp(afd->transiciones[i].estado_origen, estado_actual) == 0 && 
            afd->transiciones[i].simbolo == simbolo) {
            return afd->transiciones[i].estado_destino;
        }
    }
    return NULL; // No se encontró transición
}

// Verificar si un estado es final
int es_estado_final(AFD *afd, const char *estado) {
    for (int i = 0; i < afd->num_estados_finales; i++) {
        if (strcmp(afd->estados_finales[i], estado) == 0) {
            return 1; // Es estado final
        }
    }
    return 0; // No es estado final
}

// Simular AFD con una cadena
int simular_afd(AFD *afd, const char *cadena) {
    char estado_actual[MAX_NOMBRE_ESTADO];
    strcpy(estado_actual, afd->estado_inicial);
    
    int len = strlen(cadena);
    for (int i = 0; i < len; i++) {
        char simbolo = cadena[i];
        char *siguiente_estado = buscar_transicion(afd, estado_actual, simbolo);
        
        if (siguiente_estado == NULL) {
            return 0; // No hay transición, rechazar cadena
        }
        
        strcpy(estado_actual, siguiente_estado);
    }
    
    // Verificar si terminamos en estado final
    return es_estado_final(afd, estado_actual);
}

// Leer cadenas del archivo
int leer_cadenas(const char *archivo, char cadenas[][MAX_LINEA]) {
    FILE *file = fopen(archivo, "r");
    char linea[MAX_LINEA];
    int contador = 0;
    
    while (fgets(linea, sizeof(linea), file) && contador < MAX_CADENAS) {
        limpiar_cadena(linea);
        if (strlen(linea) > 0) {
            strcpy(cadenas[contador], linea);
            contador++;
        }
    }
    
    fclose(file);
    return contador;
}

// Mostrar información del AFD
void mostrar_afd(AFD *afd) {
    printf("Estados: ");
    for (int i = 0; i < afd->num_estados; i++) {
        printf("%s", afd->estados[i]);
        if (i < afd->num_estados - 1) printf(", ");
    }
    printf("\n");
    
    printf("Alfabeto: ");
    for (int i = 0; i < afd->tam_alfabeto; i++) {
        printf("%c", afd->alfabeto[i]);
        if (i < afd->tam_alfabeto - 1) printf(", ");
    }
    printf("\n");
    
    printf("Estado inicial: %s\n", afd->estado_inicial);
    
    printf("Estados finales: ");
    for (int i = 0; i < afd->num_estados_finales; i++) {
        printf("%s", afd->estados_finales[i]);
        if (i < afd->num_estados_finales - 1) printf(", ");
    }
    printf("\n");
    
    printf("Transiciones: ");
    for (int i = 0; i < afd->num_transiciones; i++) {
        printf("(%s,%c)->%s", afd->transiciones[i].estado_origen, 
               afd->transiciones[i].simbolo, afd->transiciones[i].estado_destino);
        if (i < afd->num_transiciones - 1) printf(", ");
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <archivo_config> <archivo_cadenas>\n", argv[0]);
        return 1;
    }
    
    AFD afd;
    char cadenas[MAX_CADENAS][MAX_LINEA];
    
    // Parsear configuración del AFD
    parsear_config(argv[1], &afd);
    
    // Mostrar información del AFD
    mostrar_afd(&afd);
    
    // Leer cadenas de prueba
    int num_cadenas = leer_cadenas(argv[2], cadenas);
    
    printf("Resultados:\n");
    for (int i = 0; i < num_cadenas; i++) {
        int resultado = simular_afd(&afd, cadenas[i]);
        printf("'%s' -> %s\n", cadenas[i], resultado ? "ACEPTA" : "NO ACEPTA");
    }
    
    return 0;
}