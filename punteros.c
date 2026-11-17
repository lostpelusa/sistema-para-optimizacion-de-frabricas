#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUCTOS 5
#define MAX_RECURSOS 10

// --- FUNCIONES DEFINIDAS ANTES DE MAIN (ELIMINA LA NECESIDAD DE PROTOTIPOS) ---

void mostrarMenu() {
    printf("\n==================================================\n");
    printf("   SISTEMA INDUSTRIAL (SIN PROTOTIPOS / PUNTEROS)  \n");
    printf("==================================================\n");
    printf("1. Configurar Fabrica y Catalogo de Recursos\n");
    printf("2. Registrar productos (con punteros)\n");
    printf("3. Mostrar inventario actual\n");
    printf("4. Editar producto (busqueda con punteros)\n");
    printf("5. Eliminar producto\n");
    printf("6. Evaluar viabilidad de produccion\n");
    printf("7. Salir\n");
    printf("--------------------------------------------------\n");
    fflush(stdout);
}

void configurarFabrica(char *nombreFabrica, float *tiempoPlantaDisponible, char *recursosGlobales, float *stockGlobal, int *numRecursosGlobales) {
    printf("\n--- CONFIGURACION INICIAL DE LA FABRICA ---\n");
    printf("Ingrese el nombre de la fabrica: ");
    fflush(stdout);
    scanf(" %[^\n]", nombreFabrica);
    while(getchar() != '\n');

    printf("Ingrese el tiempo maximo disponible de la planta (horas): ");
    fflush(stdout);
    scanf("%f", tiempoPlantaDisponible);
    while(getchar() != '\n');

    int n;
    printf("¿Cuantos tipos de recursos tendra el catalogo (max %d)?: ", MAX_RECURSOS);
    fflush(stdout);
    scanf("%d", &n);
    while(getchar() != '\n');

    if (n <= 0 || n > MAX_RECURSOS) {
        printf("[ERROR] Cantidad invalida de recursos.\n");
        fflush(stdout);
        return;
    }

    *numRecursosGlobales = 0;
    for (int i = 0; i < n; i++) {
        printf("\nRecurso [%d]:\n", i + 1);
        printf("  Nombre (ej. Cobre, Plastico): ");
        fflush(stdout);
        // Aritmética de punteros para almacenar cadenas en el bloque global
        scanf(" %[^\n]", (recursosGlobales + (i * 50)));
        while(getchar() != '\n');

        printf("  Stock maximo disponible: ");
        fflush(stdout);
        scanf("%f", (stockGlobal + i));
        while(getchar() != '\n');

        (*numRecursosGlobales)++;
    }
    printf("\n[EXITO] Fabrica configurada con %d recurso(s).\n", *numRecursosGlobales);
    fflush(stdout);
}

void ingresarProductos(char *nombresProd, float *tiemposProd, int *demandaProd, int *recursoAsociadoIndex, float *unidadesRecursoPorPieza, int *numProductos, char *recursosGlobales, int numRecursosGlobales) {
    if (numRecursosGlobales == 0) {
        printf("\n[AVISO] Configure primero la fabrica y recursos (Opcion 1).\n");
        fflush(stdout);
        return;
    }
    if (*numProductos >= MAX_PRODUCTOS) {
        printf("\n[AVISO] Capacidad maxima alcanzada (%d productos)[cite: 1].\n", MAX_PRODUCTOS);
        fflush(stdout);
        return;
    }

    int cantidadAIngresar;
    printf("\n¿Cuantos productos ingresara (max disp %d)?: ", MAX_PRODUCTOS - *numProductos);
    fflush(stdout);
    scanf("%d", &cantidadAIngresar);
    while(getchar() != '\n');

    if (cantidadAIngresar <= 0 || *numProductos + cantidadAIngresar > MAX_PRODUCTOS) {
        printf("[ERROR] Cantidad fuera de rango.\n");
        fflush(stdout);
        return;
    }

    for (int i = 0; i < cantidadAIngresar; i++) {
        int idx = *numProductos;
        printf("\n--- Producto %d ---\n", idx + 1);

        printf("Nombre del producto: ");
        fflush(stdout);
        scanf(" %[^\n]", (nombresProd + (idx * 50)));
        while(getchar() != '\n');

        printf("Tiempo de fabricacion unitario (horas): ");
        fflush(stdout);
        scanf("%f", (tiemposProd + idx));
        while(getchar() != '\n');

        printf("Demanda (unidades fisicas reales): ");
        fflush(stdout);
        scanf("%d", (demandaProd + idx));
        while(getchar() != '\n');

        printf("\n--- CATALOGO DE RECURSOS DISPONIBLES ---\n");
        for (int r = 0; r < numRecursosGlobales; r++) {
            printf("  [%d] %s\n", r + 1, (recursosGlobales + (r * 50)));
        }
        fflush(stdout);

        int seleccion;
        printf("Seleccione el numero del recurso asociado: ");
        fflush(stdout);
        scanf("%d", &seleccion);
        while(getchar() != '\n');

        if (seleccion < 1 || seleccion > numRecursosGlobales) {
            *(recursoAsociadoIndex + idx) = 0;
        } else {
            *(recursoAsociadoIndex + idx) = seleccion - 1;
        }

        printf("Cantidad de recurso requerida por pieza: ");
        fflush(stdout);
        scanf("%f", (unidadesRecursoPorPieza + idx));
        while(getchar() != '\n');

        (*numProductos)++;
    }
    printf("\n[EXITO] Producto(s) registrado(s) correctamente.\n");
    fflush(stdout);
}

void mostrarInventario(char *nombreFabrica, float tiempoPlantaDisponible, char *nombresProd, float *tiemposProd, int *demandaProd, int *recursoAsociadoIndex, float *unidadesRecursoPorPieza, int numProductos, char *recursosGlobales) {
    printf("\n========================================================================\n");
    printf(" FABRICA: %s | TIEMPO PLANTA: %.2f hrs\n", nombreFabrica, tiempoPlantaDisponible);
    printf("========================================================================\n");

    if (numProductos == 0) {
        printf("[AVISO] No hay productos registrados.\n");
        fflush(stdout);
        return;
    }

    printf("%-3s | %-12s | %-10s | %-14s | %-12s | %-8s\n", "No.", "Producto", "Tiempo/U", "Recurso Asoc.", "U. Recurso/U", "Demanda");
    printf("------------------------------------------------------------------------\n");
    for (int i = 0; i < numProductos; i++) {
        char *nombreActual = nombresProd + (i * 50);
        int rIdx = *(recursoAsociadoIndex + i);
        char *recursoActual = recursosGlobales + (rIdx * 50);

        printf("%-3d | %-12s | %-10.2f | %-14s | %-12.2f | %-8d\n", 
               i + 1, 
               nombreActual, 
               *(tiemposProd + i), 
               recursoActual, 
               *(unidadesRecursoPorPieza + i), 
               *(demandaProd + i));
    }
    printf("========================================================================\n");
    fflush(stdout);
}

void editarProducto(char *nombresProd, float *tiemposProd, int *demandaProd, int *recursoAsociadoIndex, float *unidadesRecursoPorPieza, int numProductos, char *recursosGlobales, int numRecursosGlobales) {
    if (numProductos == 0) {
        printf("\n[AVISO] No hay productos para editar.\n");
        fflush(stdout);
        return;
    }

    char busqueda[50];
    printf("\nIngrese el nombre exacto del producto a editar: ");
    fflush(stdout);
    scanf(" %[^\n]", busqueda);
    while(getchar() != '\n');

    int encontrado = -1;
    // Lógica propia de búsqueda lineal obligatoria por restricciones[cite: 1]
    for (int i = 0; i < numProductos; i++) {
        if (strcmp((nombresProd + (i * 50)), busqueda) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("\n[ERROR] Producto no encontrado.\n");
        fflush(stdout);
        return;
    }

    printf("\nProducto encontrado. Ingrese los nuevos datos:\n");
    printf("Nuevo nombre: ");
    fflush(stdout);
    scanf(" %[^\n]", (nombresProd + (encontrado * 50)));
    while(getchar() != '\n');

    printf("Nuevo tiempo unitario (horas): ");
    fflush(stdout);
    scanf("%f", (tiemposProd + encontrado));
    while(getchar() != '\n');

    printf("Nueva demanda (unidades fisicas): ");
    fflush(stdout);
    scanf("%d", (demandaProd + encontrado));
    while(getchar() != '\n');

    printf("\nCatalogo de recursos:\n");
    for (int r = 0; r < numRecursosGlobales; r++) {
        printf("  [%d] %s\n", r + 1, (recursosGlobales + (r * 50)));
    }
    fflush(stdout);

    int seleccion;
    printf("Seleccione el nuevo recurso asociado: ");
    fflush(stdout);
    scanf("%d", &seleccion);
    while(getchar() != '\n');

    if (seleccion >= 1 && seleccion <= numRecursosGlobales) {
        *(recursoAsociadoIndex + encontrado) = seleccion - 1;
    }

    printf("Nueva cantidad de recurso por pieza: ");
    fflush(stdout);
    scanf("%f", (unidadesRecursoPorPieza + encontrado));
    while(getchar() != '\n');

    printf("\n[EXITO] Producto actualizado correctamente.\n");
    fflush(stdout);
}

void eliminarProducto(char *nombresProd, float *tiemposProd, int *demandaProd, int *recursoAsociadoIndex, float *unidadesRecursoPorPieza, int *numProductos) {
    if (*numProductos == 0) {
        printf("\n[AVISO] No hay productos para eliminar.\n");
        fflush(stdout);
        return;
    }

    char busqueda[50];
    printf("\nIngrese el nombre exacto del producto a eliminar: ");
    fflush(stdout);
    scanf(" %[^\n]", busqueda);
    while(getchar() != '\n');

    int encontrado = -1;
    for (int i = 0; i < *numProductos; i++) {
        if (strcmp((nombresProd + (i * 50)), busqueda) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("\n[ERROR] Producto no encontrado.\n");
        fflush(stdout);
        return;
    }

    // Desplazamiento de memoria con punteros
    for (int i = encontrado; i < *numProductos - 1; i++) {
        strcpy((nombresProd + (i * 50)), (nombresProd + ((i + 1) * 50)));
        *(tiemposProd + i) = *(tiemposProd + (i + 1));
        *(demandaProd + i) = *(demandaProd + (i + 1));
        *(recursoAsociadoIndex + i) = *(recursoAsociadoIndex + (i + 1));
        *(unidadesRecursoPorPieza + i) = *(unidadesRecursoPorPieza + (i + 1));
    }

    (*numProductos)--;
    printf("\n[EXITO] Producto eliminado correctamente.\n");
    fflush(stdout);
}

void evaluarProduccion(float tiempoPlantaDisponible, float *tiemposProd, int *demandaProd, int *recursoAsociadoIndex, float *unidadesRecursoPorPieza, int numProductos, char *recursosGlobales, float *stockGlobal, int numRecursosGlobales) {
    if (numProductos == 0) {
        printf("\n[AVISO] No hay productos registrados para evaluar.\n");
        fflush(stdout);
        return;
    }

    float tiempoTotalReq = 0;
    float consumoPorRecurso[MAX_RECURSOS] = {0};

    for (int i = 0; i < numProductos; i++) {
        tiempoTotalReq += (*(tiemposProd + i)) * ((float)*(demandaProd + i));
        int rIdx = *(recursoAsociadoIndex + i);
        consumoPorRecurso[rIdx] += (*(unidadesRecursoPorPieza + i)) * ((float)*(demandaProd + i));
    }

    printf("\n================ RESULTADOS DEL ANALISIS ================\n");
    printf("Tiempo total de fabricacion requerido: %.2f horas\n", tiempoTotalReq);
    printf("Tiempo maximo disponible en la planta: %.2f horas\n", tiempoPlantaDisponible);
    
    int cumpleTiempo = (tiempoTotalReq <= tiempoPlantaDisponible);
    int cumpleTodosRecursos = 1;

    printf("\nAnalisis por recurso del catalogo:\n");
    for (int r = 0; r < numRecursosGlobales; r++) {
        char *nombreRec = recursosGlobales + (r * 50);
        float stockRec = *(stockGlobal + r);
        float reqRec = consumoPorRecurso[r];

        printf(" - [%s] -> Requerido: %.2f | Stock disponible: %.2f", nombreRec, reqRec, stockRec);
        
        if (reqRec > stockRec) {
            printf(" -> [DEFICIT]\n");
            cumpleTodosRecursos = 0;
        } else {
            printf(" -> [OK]\n");
        }
    }
    
    printf("-----------------------------------------------------------\n");
    if (cumpleTiempo && cumpleTodosRecursos) {
        printf("[ESTADO: VIABLE] La fabrica PUEDE cumplir totalmente con la demanda.\n");
    } else {
        printf("[ESTADO: NO VIABLE] La fabrica NO PUEDE cumplir con la demanda.\n");
        if (!cumpleTiempo) {
            printf(" -> Deficit de tiempo: Excede por %.2f horas el limite.\n", tiempoTotalReq - tiempoPlantaDisponible);
        }
        if (!cumpleTodosRecursos) {
            printf(" -> Deficit de recursos: Uno o mas insumos superan el stock disponible.\n");
        }
    }
    printf("===========================================================\n");
    fflush(stdout);
}

// --- FUNCIÓN PRINCIPAL ---

int main() {
    // Asignación dinámica de memoria mediante punteros (Heap)
    char *nombreFabrica = (char *)malloc(50 * sizeof(char));
    float *tiempoPlantaDisponible = (float *)malloc(sizeof(float));
    *tiempoPlantaDisponible = 0.0;
    strcpy(nombreFabrica, "No configurada");

    char *recursosGlobales = (char *)malloc(MAX_RECURSOS * 50 * sizeof(char));
    float *stockGlobal = (float *)malloc(MAX_RECURSOS * sizeof(float));
    int numRecursosGlobales = 0;

    char *nombresProd = (char *)malloc(MAX_PRODUCTOS * 50 * sizeof(char));
    float *tiemposProd = (float *)malloc(MAX_PRODUCTOS * sizeof(float));
    int *demandaProd = (int *)malloc(MAX_PRODUCTOS * sizeof(int));
    int *recursoAsociadoIndex = (int *)malloc(MAX_PRODUCTOS * sizeof(int));
    float *unidadesRecursoPorPieza = (float *)malloc(MAX_PRODUCTOS * sizeof(float));
    int numProductos = 0;

    if (!nombreFabrica || !recursosGlobales || !nombresProd || !tiemposProd || !demandaProd) {
        printf("Error de asignacion de memoria dinamica.\n");
        return 1;
    }

    int opcion = 0;
    do {
        mostrarMenu();
        printf("Seleccione una opcion: ");
        fflush(stdout);
        
        if (scanf("%d", &opcion) != 1) {
            printf("Entrada no valida.\n");
            fflush(stdout);
            while(getchar() != '\n'); 
            continue;
        }
        while(getchar() != '\n');

        switch (opcion) {
            case 1:
                configurarFabrica(nombreFabrica, tiempoPlantaDisponible, recursosGlobales, stockGlobal, &numRecursosGlobales);
                break;
            case 2:
                ingresarProductos(nombresProd, tiemposProd, demandaProd, recursoAsociadoIndex, unidadesRecursoPorPieza, &numProductos, recursosGlobales, numRecursosGlobales);
                break;
            case 3:
                mostrarInventario(nombreFabrica, *tiempoPlantaDisponible, nombresProd, tiemposProd, demandaProd, recursoAsociadoIndex, unidadesRecursoPorPieza, numProductos, recursosGlobales);
                break;
            case 4:
                editarProducto(nombresProd, tiemposProd, demandaProd, recursoAsociadoIndex, unidadesRecursoPorPieza, numProductos, recursosGlobales, numRecursosGlobales);
                break;
            case 5:
                eliminarProducto(nombresProd, tiemposProd, demandaProd, recursoAsociadoIndex, unidadesRecursoPorPieza, &numProductos);
                break;
            case 6:
                evaluarProduccion(*tiempoPlantaDisponible, tiemposProd, demandaProd, recursoAsociadoIndex, unidadesRecursoPorPieza, numProductos, recursosGlobales, stockGlobal, numRecursosGlobales);
                break;
            case 7:
                printf("\nSaliendo del programa... ¡Hasta luego!\n");
                fflush(stdout);
                break;
            default:
                printf("\nOpcion invalida.\n");
                fflush(stdout);
        }
    } while (opcion != 7);

    // Liberación de memoria dinámica
    free(nombreFabrica);
    free(tiempoPlantaDisponible);
    free(recursosGlobales);
    free(stockGlobal);
    free(nombresProd);
    free(tiemposProd);
    free(demandaProd);
    free(recursoAsociadoIndex);
    free(unidadesRecursoPorPieza);

    return 0;
}