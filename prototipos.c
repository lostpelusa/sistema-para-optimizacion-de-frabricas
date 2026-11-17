#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUCTOS 5
#define MAX_RECURSOS 10

// Prototipos de funciones
void mostrarMenu();
void configurarFabrica(char nombreFabrica[], float *tiempoPlantaDisponible, char recursosGlobales[][50], float stockGlobal[], int *numRecursosGlobales);
void ingresarProductos(char nombresProd[][50], float tiemposProd[], int demandaProd[], int recursoAsociadoIndex[], float unidadesRecursoPorPieza[], int *numProductos, char recursosGlobales[][50], int numRecursosGlobales);
void mostrarInventario(char nombreFabrica[], float tiempoPlantaDisponible, char nombresProd[][50], float tiemposProd[], int demandaProd[], int recursoAsociadoIndex[], float unidadesRecursoPorPieza[], int numProductos, char recursosGlobales[][50]);
void editarProducto(char nombresProd[][50], float tiemposProd[], int demandaProd[], int recursoAsociadoIndex[], float unidadesRecursoPorPieza[], int numProductos, char recursosGlobales[][50], int numRecursosGlobales);
void eliminarProducto(char nombresProd[][50], float tiemposProd[], int demandaProd[], int recursoAsociadoIndex[], float unidadesRecursoPorPieza[], int *numProductos);
void evaluarProduccion(float tiempoPlantaDisponible, float tiemposProd[], int demandaProd[], int recursoAsociadoIndex[], float unidadesRecursoPorPieza[], int numProductos, char recursosGlobales[][50], float stockGlobal[], int numRecursosGlobales);

int main() {
    char nombreFabrica[50] = "No configurada";
    float tiempoPlantaDisponible = 0.0;
    char recursosGlobales[MAX_RECURSOS][50];
    float stockGlobal[MAX_RECURSOS];
    int numRecursosGlobales = 0;

    char nombresProd[MAX_PRODUCTOS][50];
    float tiemposProd[MAX_PRODUCTOS];
    int demandaProd[MAX_PRODUCTOS];
    int recursoAsociadoIndex[MAX_PRODUCTOS];
    float unidadesRecursoPorPieza[MAX_PRODUCTOS];
    int numProductos = 0;

    int opcion = 0;

    do {
        mostrarMenu();
        printf("Seleccione una opcion: ");
        fflush(stdout);
        
        if (scanf("%d", &opcion) != 1) {
            printf("Entrada no valida. Intente de nuevo.\n");
            fflush(stdout);
            while(getchar() != '\n'); 
            continue;
        }
        while(getchar() != '\n'); // Limpiar buffer tras leer la opción

        switch (opcion) {
            case 1:
                configurarFabrica(nombreFabrica, &tiempoPlantaDisponible, recursosGlobales, stockGlobal, &numRecursosGlobales);
                break;
            case 2:
                ingresarProductos(nombresProd, tiemposProd, demandaProd, recursoAsociadoIndex, unidadesRecursoPorPieza, &numProductos, recursosGlobales, numRecursosGlobales);
                break;
            case 3:
                mostrarInventario(nombreFabrica, tiempoPlantaDisponible, nombresProd, tiemposProd, demandaProd, recursoAsociadoIndex, unidadesRecursoPorPieza, numProductos, recursosGlobales);
                break;
            case 4:
                editarProducto(nombresProd, tiemposProd, demandaProd, recursoAsociadoIndex, unidadesRecursoPorPieza, numProductos, recursosGlobales, numRecursosGlobales);
                break;
            case 5:
                eliminarProducto(nombresProd, tiemposProd, demandaProd, recursoAsociadoIndex, unidadesRecursoPorPieza, &numProductos);
                break;
            case 6:
                evaluarProduccion(tiempoPlantaDisponible, tiemposProd, demandaProd, recursoAsociadoIndex, unidadesRecursoPorPieza, numProductos, recursosGlobales, stockGlobal, numRecursosGlobales);
                break;
            case 7:
                printf("\nSaliendo del programa... ¡Hasta luego!\n");
                fflush(stdout);
                break;
            default:
                printf("\nOpcion invalida. Intente nuevamente.\n");
                fflush(stdout);
        }
    } while (opcion != 7);

    return 0;
}

void mostrarMenu() {
    printf("\n==================================================\n");
    printf("   SISTEMA DE OPTIMIZACION DE FABRICA INDUSTRIAL   \n");
    printf("==================================================\n");
    printf("1. Configurar Fabrica (Nombre, Tiempo y Recursos)\n");
    printf("2. Registrar productos y asociar recursos\n");
    printf("3. Mostrar inventario y configuracion actual\n");
    printf("4. Editar producto (busqueda manual propia)\n");
    printf("5. Eliminar producto (busqueda manual propia)\n");
    printf("6. Evaluar viabilidad de produccion (Tiempo y Stocks)\n");
    printf("7. Salir\n");
    printf("--------------------------------------------------\n");
    fflush(stdout);
}

void configurarFabrica(char nombreFabrica[], float *tiempoPlantaDisponible, char recursosGlobales[][50], float stockGlobal[], int *numRecursosGlobales) {
    printf("\n--- CONFIGURACION INICIAL DE LA FABRICA ---\n");
    printf("Ingrese el nombre de la fabrica: ");
    fflush(stdout);
    scanf(" %[^\n]", nombreFabrica);
    while(getchar() != '\n');

    printf("Ingrese el tiempo maximo disponible de produccion de la planta (horas): ");
    fflush(stdout);
    scanf("%f", tiempoPlantaDisponible);
    while(getchar() != '\n');

    int n;
    printf("¿Cuantos tipos de recursos manejara el catalogo global (max %d)?: ", MAX_RECURSOS);
    fflush(stdout);
    scanf("%d", &n);
    while(getchar() != '\n');

    if (n <= 0 || n > MAX_RECURSOS) {
        printf("[ERROR] Cantidad de recursos invalida.\n");
        fflush(stdout);
        return;
    }

    *numRecursosGlobales = 0;
    for (int i = 0; i < n; i++) {
        printf("\nRecurso global [%d]:\n", i + 1);
        printf("  Nombre del recurso (ej. Cobre, Plastico, Metal): ");
        fflush(stdout);
        scanf(" %[^\n]", recursosGlobales[i]);
        while(getchar() != '\n');

        printf("  Cantidad maxima disponible en stock global: ");
        fflush(stdout);
        scanf("%f", &stockGlobal[i]);
        while(getchar() != '\n');

        (*numRecursosGlobales)++;
    }
    printf("\n[EXITO] Fabrica, tiempo disponible (%.2f hrs) y recursos registrados correctamente.\n", *tiempoPlantaDisponible);
    fflush(stdout);
}

void ingresarProductos(char nombresProd[][50], float tiemposProd[], int demandaProd[], int recursoAsociadoIndex[], float unidadesRecursoPorPieza[], int *numProductos, char recursosGlobales[][50], int numRecursosGlobales) {
    if (numRecursosGlobales == 0) {
        printf("\n[AVISO] Primero debe configurar los datos de la fabrica y catalogo (Opcion 1).\n");
        fflush(stdout);
        return;
    }
    if (*numProductos >= MAX_PRODUCTOS) {
        printf("\n[AVISO] Capacidad maxima alcanzada (%d productos)[cite: 1].\n", MAX_PRODUCTOS);
        fflush(stdout);
        return;
    }

    int cantidadAIngresar;
    printf("\n¿Cuantos productos desea ingresar (max disponible %d)?: ", MAX_PRODUCTOS - *numProductos);
    fflush(stdout);
    scanf("%d", &cantidadAIngresar);
    while(getchar() != '\n');

    if (cantidadAIngresar <= 0 || *numProductos + cantidadAIngresar > MAX_PRODUCTOS) {
        printf("[ERROR] Cantidad fuera de rango permitido.\n");
        fflush(stdout);
        return;
    }

    for (int i = 0; i < cantidadAIngresar; i++) {
        int index = *numProductos;
        printf("\n--- Producto %d ---\n", index + 1);

        printf("Nombre del producto: ");
        fflush(stdout);
        scanf(" %[^\n]", nombresProd[index]);
        while(getchar() != '\n');

        printf("Tiempo de fabricacion por unidad (horas): ");
        fflush(stdout);
        scanf("%f", &tiemposProd[index]);
        while(getchar() != '\n');

        printf("Cantidad demandada (unidades fisicas reales a producir): ");
        fflush(stdout);
        scanf("%d", &demandaProd[index]);
        while(getchar() != '\n');

        printf("\n--- CATALOGO DE RECURSOS REGISTRADOS ---\n");
        for (int r = 0; r < numRecursosGlobales; r++) {
            printf("  [%d] %s\n", r + 1, recursosGlobales[r]);
        }
        fflush(stdout);

        printf("Seleccione el numero del recurso necesario para '%s': ", nombresProd[index]);
        fflush(stdout);
        int seleccion;
        scanf("%d", &seleccion);
        while(getchar() != '\n');

        if (seleccion < 1 || seleccion > numRecursosGlobales) {
            printf("[AVISO] Seleccion fuera de rango. Se asigno la opcion 1 por defecto.\n");
            recursoAsociadoIndex[index] = 0;
        } else {
            recursoAsociadoIndex[index] = seleccion - 1;
        }

        printf("Cantidad de '%s' requerida por cada pieza producida: ", recursosGlobales[recursoAsociadoIndex[index]]);
        fflush(stdout);
        scanf("%f", &unidadesRecursoPorPieza[index]);
        while(getchar() != '\n');

        (*numProductos)++;
    }
    printf("\n[EXITO] Productos registrados correctamente.\n");
    fflush(stdout);
}

void mostrarInventario(char nombreFabrica[], float tiempoPlantaDisponible, char nombresProd[][50], float tiemposProd[], int demandaProd[], int recursoAsociadoIndex[], float unidadesRecursoPorPieza[], int numProductos, char recursosGlobales[][50]) {
    printf("\n========================================================================\n");
    printf(" FABRICA: %s | TIEMPO PLANTA: %.2f hrs\n", nombreFabrica, tiempoPlantaDisponible);
    printf("========================================================================\n");

    if (numProductos == 0) {
        printf("[AVISO] No hay productos registrados en el sistema.\n");
        fflush(stdout);
        return;
    }

    printf("%-3s | %-12s | %-10s | %-14s | %-12s | %-8s\n", "No.", "Producto", "Tiempo/U", "Recurso Asoc.", "U. Recurso/U", "Demanda");
    printf("------------------------------------------------------------------------\n");
    for (int i = 0; i < numProductos; i++) {
        printf("%-3d | %-12s | %-10.2f | %-14s | %-12.2f | %-8d\n", 
               i + 1, 
               nombresProd[i], 
               tiemposProd[i], 
               recursosGlobales[recursoAsociadoIndex[i]], 
               unidadesRecursoPorPieza[i], 
               demandaProd[i]);
    }
    printf("========================================================================\n");
    fflush(stdout);
}

void editarProducto(char nombresProd[][50], float tiemposProd[], int demandaProd[], int recursoAsociadoIndex[], float unidadesRecursoPorPieza[], int numProductos, char recursosGlobales[][50], int numRecursosGlobales) {
    if (numProductos == 0) {
        printf("\n[AVISO] No hay productos para editar.\n");
        fflush(stdout);
        return;
    }

    char busqueda[50];
    printf("\nIngrese el nombre exacto del producto a buscar para editar: ");
    fflush(stdout);
    scanf(" %[^\n]", busqueda);
    while(getchar() != '\n');

    int encontrado = -1;
    for (int i = 0; i < numProductos; i++) {
        if (strcmp(nombresProd[i], busqueda) == 0) {
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
    scanf(" %[^\n]", nombresProd[encontrado]);
    while(getchar() != '\n');

    printf("Nuevo tiempo unitario (horas): ");
    fflush(stdout);
    scanf("%f", &tiemposProd[encontrado]);
    while(getchar() != '\n');

    printf("Nueva demanda (unidades fisicas): ");
    fflush(stdout);
    scanf("%d", &demandaProd[encontrado]);
    while(getchar() != '\n');

    printf("\nCatalogo de recursos:\n");
    for (int r = 0; r < numRecursosGlobales; r++) {
        printf("  [%d] %s\n", r + 1, recursosGlobales[r]);
    }
    fflush(stdout);

    int seleccion;
    printf("Seleccione el nuevo recurso asociado: ");
    fflush(stdout);
    scanf("%d", &seleccion);
    while(getchar() != '\n');
    
    if (seleccion >= 1 && seleccion <= numRecursosGlobales) {
        recursoAsociadoIndex[encontrado] = seleccion - 1;
    }

    printf("Nueva cantidad de recurso requerida por pieza: ");
    fflush(stdout);
    scanf("%f", &unidadesRecursoPorPieza[encontrado]);
    while(getchar() != '\n');

    printf("\n[EXITO] Producto actualizado correctamente.\n");
    fflush(stdout);
}

void eliminarProducto(char nombresProd[][50], float tiemposProd[], int demandaProd[], int recursoAsociadoIndex[], float unidadesRecursoPorPieza[], int *numProductos) {
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
        if (strcmp(nombresProd[i], busqueda) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado == -1) {
        printf("\n[ERROR] Producto no encontrado.\n");
        fflush(stdout);
        return;
    }

    for (int i = encontrado; i < *numProductos - 1; i++) {
        strcpy(nombresProd[i], nombresProd[i + 1]);
        tiemposProd[i] = tiemposProd[i + 1];
        demandaProd[i] = demandaProd[i + 1];
        recursoAsociadoIndex[i] = recursoAsociadoIndex[i + 1];
        unidadesRecursoPorPieza[i] = unidadesRecursoPorPieza[i + 1];
    }

    (*numProductos)--;
    printf("\n[EXITO] Producto eliminado correctamente.\n");
    fflush(stdout);
}

void evaluarProduccion(float tiempoPlantaDisponible, float tiemposProd[], int demandaProd[], int recursoAsociadoIndex[], float unidadesRecursoPorPieza[], int numProductos, char recursosGlobales[][50], float stockGlobal[], int numRecursosGlobales) {
    if (numProductos == 0) {
        printf("\n[AVISO] No hay productos registrados para evaluar.\n");
        fflush(stdout);
        return;
    }

    float tiempoTotalReq = 0;
    float consumoPorRecurso[MAX_RECURSOS] = {0};

    for (int i = 0; i < numProductos; i++) {
        tiempoTotalReq += tiemposProd[i] * (float)demandaProd[i];
        int rIdx = recursoAsociadoIndex[i];
        consumoPorRecurso[rIdx] += unidadesRecursoPorPieza[i] * (float)demandaProd[i];
    }

    printf("\n================ RESULTADOS DEL ANALISIS ================\n");
    printf("Tiempo total de fabricacion requerido: %.2f horas\n", tiempoTotalReq);
    printf("Tiempo maximo disponible en la planta: %.2f horas\n", tiempoPlantaDisponible);
    
    int cumpleTiempo = (tiempoTotalReq <= tiempoPlantaDisponible);
    int cumpleTodosRecursos = 1;

    printf("\nAnalisis por recurso del catalogo:\n");
    for (int r = 0; r < numRecursosGlobales; r++) {
        printf(" - [%s] -> Requerido: %.2f | Stock disponible: %.2f", 
               recursosGlobales[r], consumoPorRecurso[r], stockGlobal[r]);
        
        if (consumoPorRecurso[r] > stockGlobal[r]) {
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
            printf(" -> Deficit de tiempo: Excede por %.2f horas el limite de la planta.\n", tiempoTotalReq - tiempoPlantaDisponible);
        }
        if (!cumpleTodosRecursos) {
            printf(" -> Deficit de recursos: Uno o mas insumos superan el stock maximo disponible.\n");
        }
    }
    printf("===========================================================\n");
    fflush(stdout);
}