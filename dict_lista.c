#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Dict {
	char palIngles  [50];
	char palEspanol [50];
};
typedef struct Dict tipoDict;

struct Nodo {
	tipoDict info;
	struct Nodo *siguiente;
};
typedef struct Nodo tipoNodo;

tipoNodo *crearNodo (tipoDict dict) {
	tipoNodo *nodo = (tipoNodo *)malloc(sizeof(tipoNodo));
	strcpy (nodo->info.palIngles, dict.palIngles);
	strcpy (nodo->info.palEspanol, dict.palEspanol);
	nodo->siguiente = NULL;
	return nodo;
}

tipoNodo* adicionarLista (tipoNodo *lista, tipoDict dict){
	tipoNodo *nuevoNodo = crearNodo (dict);
	if (lista==NULL) {
		lista = nuevoNodo;
	}else {
		tipoNodo *aptNodo = lista;
		while (aptNodo->siguiente != NULL) {
			aptNodo = aptNodo->siguiente;
		}
		aptNodo->siguiente = nuevoNodo;
	}
	return lista;
}

tipoNodo* cargarDictToLista (char *nombreArchivoBin, tipoNodo *lista){
	FILE *manejadorBin;
	manejadorBin = fopen (nombreArchivoBin, "r");
	tipoDict dictTmp;
	int longitudBytes = sizeof (tipoDict);

	while (fread (&dictTmp, longitudBytes, 1, manejadorBin) == 1) {
		lista = adicionarLista (lista, dictTmp);
	}
	fclose (manejadorBin);
	return lista;
}

char *Buscar_palabra(char *palabra,tipoNodo *list){//devuelve un char cuidado
    tipoNodo *p;//esto e spara guardar la dirrecion de memoria

	char* P_i= (char*) malloc ((sizeof(char)) *50);
	char* P_e = (char*) malloc ((sizeof(char)) *50);
    if(list == NULL){
        return "no existe";//en otro casos pues no existe
    }
    else{
        p=list;
        while(p!=NULL){
            strcpy(P_i,p->info.palIngles);
            strcpy(P_e,p->info.palEspanol);

            if(strcmp(palabra,P_i)==0){
                return P_e;
            }
            else if(strcmp(palabra,P_e)==0){
                return P_i;
            }
            else{
                p=p->siguiente;
            }
        }
        return "La palabra no se encontro";
    }
}

void Buscar_palabra_inteligente(){//devuelve un char cuidado
    FILE * Diccionario = fopen("palabras.txt","r");
    char guardar[50];
    char palabra[50];//lo que deseo buscar
    printf("Digite la palabra que desea buscar: ");
    scanf("%s",&palabra);//tomo lo que me mandan  escriben.
    if(Diccionario==NULL){
        printf("No se pudo leer: :( .....");
        exit(0);
    }
    else{
        while(!(feof(Diccionario))){
            fgets(guardar,50,Diccionario);//donde se guarda,espacio,de donde lo lee.
            char *x;
            x=strstr(guardar,palabra);
            if(strstr(guardar,palabra)!=NULL){//strstr
                strtok(x," : ");
                strtok(NULL," ");
                printf("%s\n",x);
            }
        }
    }
    fclose(Diccionario);

}

int buscarPos(char *palabra,tipoNodo *lista ){
	int pos = 0;
	if (lista == NULL){
        printf("LISTA VACIA");
	}
	tipoNodo *aptNodo = lista;
	while (aptNodo!= NULL){
		if (strcmp(palabra, aptNodo -> info.palIngles) > 0){
			pos ++;
			aptNodo = aptNodo->siguiente;
		}
		else
			break;
	}
	return pos ;
}


tipoNodo *insertarlista(tipoNodo *lista,int pos,tipoDict elemento){
    tipoNodo *temp=crearNodo(elemento);//retorna un nodo con espacio para la palabra
    tipoNodo *p=lista;//pu tero al primer nodo
    int contador=1;
    if(pos==1){//en caso de que sea igual a(a) casi imposible
            temp->siguiente=lista;
            lista=temp;
    }
    else{
        while(contador<pos-1){
            contador++;
            p=p->siguiente;
        }
        temp->siguiente=p->siguiente;
        p->siguiente=temp;
    }
    return lista;
}

tipoNodo *Modificar(tipoNodo *lista,char *palabra){

    int pos = buscarPos(palabra,lista);
	tipoNodo *p_nodo = lista;
	int i=1;
	char palabra_e[50];
    printf("ingrese cual quiere que ahora desea que sea la nueva traduccion: ");
    scanf("%s",palabra_e);
	if(pos==1){
        strcpy(lista->info.palEspanol,palabra_e);
	}
	else{
        while (i <= pos-1){//seria lo mismo que i < pos ?????///mirar la logica si llega

            //if(strcmp(palabra,p_nodo->info.palIngles)!=0){
            i++;
            p_nodo=p_nodo->siguiente;
        }
        strcpy(p_nodo->info.palEspanol,palabra_e);
        }
	return lista;
}

tipoNodo* adicionar(char* esp, char* ing,tipoNodo *lista ){//revisar esta
	tipoDict nuevo;
	tipoNodo* nodo;
	int i;
	strcpy(nuevo.palIngles,ing);
	strcpy(nuevo.palEspanol,esp);

	nodo=crearNodo(nuevo);
	i= buscarPos(ing,lista);
	tipoNodo* result= insertarlista(lista,i,nuevo);
	return result;
}

tipoNodo* eliminar(char* palabra, tipoNodo* lista){//si algo separar la palabra de entrada por psicion y hacer una funcion para la plabra
	tipoNodo* tmp, *q;
	int pos= buscarPos(palabra,lista);//posicion
	if (pos == 1){
	    lista=lista->siguiente;
        tmp=lista;
        return tmp;
	}
	else{
		int contador=1;  // 9 = 10-9 =9
		tmp=lista;
		while(contador < pos-1){
                contador++;
			tmp=tmp->siguiente;
		}
		q= tmp->siguiente;
		tmp->siguiente=tmp->siguiente->siguiente;
		free(q);
	return tmp;
	}
}

void imprimirLista (tipoNodo *lista) {
	tipoNodo *aptNodo = lista;
	while (aptNodo != NULL) {
		printf ("<%s>::<%s>\n", aptNodo->info.palIngles, aptNodo->info.palEspanol);
		aptNodo = aptNodo->siguiente;
	}
}

void guardarListaArchivo(tipoNodo *lista){
    FILE *archivo_binario=fopen("nombreArchivo.bin","wb");
    FILE *archivo_texto=fopen("nombreArchivo.txt","w");

    char cadena[150];
    tipoNodo *p =lista;
    tipoDict *tmpInfo;
    while(p!=NULL){
            tmpInfo= &(p->info);
            //printf ("%s:%s\n", tmpInfo->palIngles,tmpInfo->palEspanol);
            fwrite(tmpInfo,sizeof(tipoDict),1,archivo_binario);
            sprintf(cadena,"<%s>:<%s>\n",tmpInfo->palIngles,tmpInfo->palEspanol);
            fputs(cadena,archivo_texto);//poner en el archivo texto
            p=p->siguiente;
    }

    fclose(archivo_binario);
    fclose(archivo_texto);
}


void menu(tipoNodo *lista){//colocar como areglos para no colcoar los &

    int opcion;//las opciones de todo el programa.
    char pal[50];
    char *traduccion;//traduccion de la palbra
    //////adicionar una un nodo a la lista o diccionario
    tipoDict elemento;
    char palabraingles[50];
    char palabraespanol[50];
    int posicion_adicionar;
    /////////////////////////
    char pal_eliminar[50];
    ////////////////////////
    char pal_modificar[50];
    //////////////////////

    do{
    printf("\n\tESTE ES EL MENU\n\tQUE DESEA REALIZAR?\n");
	printf("\n1: Busqueda exacta\n2: adicionar palabra\n3: eliminar palabra\n4: Modificar palabra\n5: salir\n6:Guardar cambios\n7:Buscar palabra inteligente.\n8:Imprimir lista\n");
	scanf("%d",&opcion);
	switch(opcion){
		case 1:
            printf("ingrese la palabra:\n");
            scanf("%s",pal);
            traduccion=Buscar_palabra(pal,lista);//para buscar la palabra
            printf("---------------------------------------------------------\n");
            printf("\tLA PALABRA ES: <%s>:<%s>\n",pal,traduccion);
            printf("---------------------------------------------------------");
			break;
		case 2://revisar esto
		    printf("ingrese la palabra en ingles:\n");
            scanf("%s",palabraingles);
            printf("ingrese la palabra en español:\n");
            scanf("%s",palabraespanol);

            lista=adicionar(palabraespanol,palabraingles,lista);

			break;
		case 3:
		    printf("ingrese la palabra que desea eliminar:\n");
            scanf("%s",pal_eliminar);
		    lista=eliminar(pal_eliminar,lista);
			break;
		case 4:
		    printf("ingrese la palabra a modificar:\n");
            scanf("%s",pal_modificar);
            lista=Modificar(lista,pal_modificar);
			break;
		case 5:
		    printf("---------------------------------------------------------\n");
		    printf("\n Adios y Gracias ;) \n");
		    printf("---------------------------------------------------------");
			break;
        case 6:
            guardarListaArchivo(lista);
			break;
        case 7:
            Buscar_palabra_inteligente();
            break;
        case 8:
            imprimirLista(lista);
            break;
        default:
			printf("\n Esta opcion no existe!\n Intentalo nuevamente. \n");

	}
    }while (opcion!=5);
}

int main(){
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    tipoNodo *lista;                                                               //
	tipoDict dict;
	                                                                  //
	lista = NULL;                                                                                 //
	lista = cargarDictToLista ("palabras.bin", lista);                                                       //
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    menu(lista);



	return 0;
}
