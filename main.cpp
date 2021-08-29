#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <vector>
#include <iostream>
#include <stack>

using namespace std;

int filas = 0;
int cols = 0;

void imprimeMatriz(int **matriz) {
	for (int i = 0; i < filas; i++) {
		for (int j = 0; j < cols; j++) {
			printf("%d\t", matriz[i][j]);
		}
		printf("\n");
	}
}

int** leerMatriz(char *fileName) { //Metodo que regresa una matriz
	FILE *fp = fopen(fileName, "r"); //Se declara el apuntador al archivo
	if (fp == NULL) {
		exit(EXIT_FAILURE);
	}
	char linea[80];
	fgets(linea, 79, fp); //fgets dice donde se va a guardar
	char *token = strtok(linea, " ");//La cadena que quiero obtener 
	filas = atoi(token); //Convierte el valor que se lee a un entero
	token = strtok(NULL, " "); //Toma el siguiente valor y se guarda en columnas
	cols = atoi(token);
	int **matriz = new int*[filas];
	for (int i = 0; i < filas; i++) { //Se leen las filas
		matriz[i] = new int[cols];
		fgets(linea,79, fp); // Se lee la linea
		token = strtok(linea, " ");
		matriz[i][0] = atoi(token); //El valor que se lee en I,0 se toma desde el primer token
		for (int j = 1; j < cols; j++) { //Se leen las columnas
			token = strtok(NULL, " ");
			matriz[i][j] = atoi(token);
		}
	}
	fclose(fp);
	return matriz;
}

void solVoraz(int **matriz)
{
	
	stack<int> mRes[filas]; //Arreglo de pilas, estructura auxiliar
	int villas = 0; // Numero de villas que se tendran que utilizar
	
	
	for (int i=0;i<filas;i++)
	{
		for(int j=i;j<cols;j++)
		{
			bool senal = false; //La variable señal, es la que nos dice si puedo incluir un elemento en una villa o no, si es false se puede, si es verdadera no
			if(matriz[i][j]==0)
			{
				for (int r=0;r<=j;r++) //Si dos elementos tienen buenas relaciones, empiezo a evaluar las relaciones de mi candidato con los que ya estan en la villa
				{
					if(matriz[r][j]==1 && matriz[i][r]==0){
						senal = true; //Si el candidato no tiene buenas relaciones con algun elemento que mi objetivo ya tenga como compañero
						break; //interrumpo y evaluo el siguiente candidato
					}
				}
				if(senal == false){ //Si el candidato tiene buenas relaciones con las selecciones en la pila objetivo
					mRes[i].push(j); //lo inserto en esa pila
					for(int r=i+1;r<filas;r++)
					{
						matriz[r][j]=1; //y elimino las relaciones de ese elemento que meto a la pila, para que no pueda entrar
										//a otra villa
					}
				}
				
			}
		}
	}
	
	for(int i=0;i<filas;i++)
	{
		if(!mRes[i].empty())
		{
			villas++; //Evaluo las posiciones del arreglo no vacias para saber cuantas villas son en total
		}
	}
	
	
	int **arreglo = new int*[villas];
	for (int i = 0; i < villas; i++) {
    	arreglo[i] = new int[filas]; //Creo la matriz de villas x selecciones
	}
	
	for (int i=0;i<villas;i++)
	{
		for (int j=0;j<filas;j++)
		{
			arreglo[i][j]=0; // Relleno la matriz de ceros
		}
	}

	cout<<"Asi se deben ubicar las delegaciones"<<endl;
	int l=0;
	int k=0;
	for(int i=0;i<filas;i++)
	{
		if(!mRes[i].empty())
		{
			while(!mRes[i].empty())
				{	
					l= mRes[i].top(); //Voy recorriendo las pilas del arreglo que no estan vacias
					arreglo[k][l]=1; //y voy marcando en mi matriz de respuestas
					mRes[i].pop();
				}
			k++; //Paso a la siguiente solo cuando la posicion del arreglo no es una pila vacia	
		}			
	}
	
	for (int i=0;i<villas;i++)
	{
		for (int j=0;j<filas;j++)
		{
			cout<<arreglo[i][j]<<" "; //Finalmente imprimo la matriz de respuesta
		}
		cout<<endl;
	}	
}


void solIngenua(int **matriz)
{
	stack<int> aux; //Esta es la estructura que me ayudara con las selecciones objetivo
	int mAux[filas][filas]={{0}};	//Esta sera mi matriz auxiliar en la que consignare las posibles villas
	
	for(int i=0;i<filas;i++)
	{
		for(int j=0;j<filas;j++)
		{
			mAux[i][j] = 0; //Inicializo mi matriz auxiliar
		}
	}
	
	int k=0;
	int v=0;
	for(int i=filas-1;i>=0;i--)
	{
		aux.push(i); //Relleno la pila de manera descendente para que el primer elemento en ser evaluado sea el 0
	}
	
	while(!aux.empty()) //Mientras haya elemento para evaluar ... aqui empiezo a recorrer la pila
	{
		
		for(int i=aux.top();i<filas;i++) //Miro mi matriz principal en la posicion (objetivo, colomna)
		{
			
			if(matriz[aux.top()][i]==0) //Si encuentro un elemento con buenas relaciones
			{
				mAux[k][i]=1;  /// lleno la matriz aux de donde sacare las villas
				for(int m=i;m<filas;m++)
				{
					if(i!=aux.top() && matriz[i][m]==1)
					{
						matriz[aux.top()][m]=1;// inhabilito las siguientes naciones que se quieran con la principal, pero no con la entrante
					}
				}
				for(int r=aux.top()+1;r<filas;r++)
					{
						matriz[r][i]=1; //Inhabilito las selecciones que meto a la matriz aux, para que no pueda estar en otra villa
					}	
			}
		}
		aux.pop(); //Paso al siguiente objetivo
		k++; //Posicion filas de la matriz auxiliar
	}
	
	bool senal=true;
	for(int i=0;i<filas;i++) //aqui miro cuantas villas obtuve
	{
		senal=true;
		for(int j=0;j<filas;j++)
		{
			if(senal==false)
			{
				continue;
			}
			if(mAux[i][j]==1)
			{
				senal = false;
				v++; //numero de villas que va aumentado
			}
		}
	}
	
	cout<<"villas "<<v<<endl;
	
	int villas[v][filas]={{0}};
	
	for(int i=0;i<v;i++)
	{
		for(int j=0;j<filas;j++)
		{
			villas[i][j] = 0;
		}
	}
	
	int m=0;
	senal=true;
	
	for(int i=0;i<filas;i++)
	{
		for(int j=0;j<filas;j++)
		{
			if(mAux[i][j]==1) //con esto lleno mi matriz villas x selecciones
			{
				villas[m][j]=1;
				senal=false;
			}	
		}
		if(senal==false)
		{
			m++; //la señal solo aumenta cuando hay una fila que tiene elementos en 1, como habran filas que quedan en 0
			senal=true; //debo utilizar una señal
		}
		
	}
	
	for(int i=0;i<v;i++)
	{
		for(int j=0;j<filas;j++)
		{
			cout<<villas[i][j]<<" ";
		}
		cout<<endl;
	}	
}


void solDinamica(int **matriz)
{
	int estructura[filas]={0}; //Arreglo donde estan todas las selecciones 
	vector<int> temporal; //Este sera el vector que me ayudara a validar las que ingresare
	int k = 0; // me ayudara a manejar la iteracion de la matriz en sus filas
	int i = 0; // me ayudara a manejar la iteracion en las colomnas
	int villas = 1; // estas seran las villas
	
	
	 
	while(k<filas)
	{
		if(matriz[k][i]==0 && estructura[i] == 0) //Pregunto por cada posicion en la matriz y si no tienen aun villa asignada
		{
			int count =0; // Este count me ayudara a saber de cuantas selecciones en temporal es amiga la candidata
			if(temporal.empty())
			{
				temporal.push_back(i); //si mi ventor temporal esta vacio, es porque estoy evaluando el objetivo
				matriz[k][i]=1; //Inhabilito esa posicion en la matriz
				estructura[i]=villas; //En mi estructura de memoria asigno la etiqueta del numero de villa a la seleccion
			}else
			{
				for(int j=0;j<temporal.size();j++) //recorro todo el vector temporal
				{
					if(matriz[temporal[j]][i]==0)
					{
						count++; //cuento cuantos de los elementos de ese vector temporal son amigos de mi candidato
					}	
				}
				if(count==temporal.size())
				{
					temporal.push_back(i); //si es amigo de todos, lo agrego al vector temporal
					matriz[k][i]=1; // inhabilito la posicion de la matriz
					estructura[i]=villas; //asigno la etiqueta de la villa a la que pertenece
				}
			}
		}
		i++; // me muevo a la siguiente columna
		if(i==filas)
		{
			k++; //cambio de fila osea de seleccion objetivo
			i=k; //igual i a k para que empiece en las posiciones iguales y solo evalue la triangular superior
			if(!temporal.empty())
			{
				villas++; //Cambio a la siguiente etiqueta osea la siguiente villa
			}
			temporal.clear(); //formeto el temporal
		}
	}
	cout<<"Se ubican en: "<<villas-1<<" villas"<<endl;
	
	int mayor =0;
	for(int i=0;i<filas;i++)
	{
		if(estructura[i]>mayor)
		{
			mayor = estructura[i]; //debo extraer el elemento mayor de la estructura de memoria, esas son mis villas
		}	
	}
	
	int solVillas[mayor][filas]={{0}}; // creo mi matriz solucion villas x selecciones
	for(int i=0;i<mayor;i++)
	{
		for(int j=0;j<filas;j++)
		{
			solVillas[i][j]=0; // La inicializa en 0
		}
	}
	
	for(int i=0;i<filas;i++)
	{
		solVillas[estructura[i]-1][i]=1; //cotejo las selecciones con la etiqueta puesta en la estructura de memoria
										//y marco los unos correspondientes en cada villa
	}
	
	for(int i=0;i<mayor;i++)
	{
		for(int j=0;j<filas;j++) //imprimo la solucion
		{
			cout<<solVillas[i][j]<<" ";
		}
		cout<<endl;
	}	
}

int main(int argc, char** argv) {
	int opc=0;
	int **M2 = leerMatriz("prueba.txt");
	printf("LO QUE SE LEE DEL ARCHIVO: \n");
	imprimeMatriz(M2);
	
	printf("Bienvenido, escoja el metodo por el cual quiere solucionar el problema: \n");
	while(opc!=4)
	{
	
	int **M2 = leerMatriz("prueba.txt");
	printf("1. Solucion Ingenua \n");
	printf("2. Solucion Voraz \n");
	printf("3. Solucion Dinamica \n");
	printf("4. salir \n");
	std::cin>>opc;

	switch(opc)
	{
		case 1: printf("La solucion Ingenua es: \n");
				solIngenua(M2);break;
		case 2: printf("La solucion Voraz es: \n");
				solVoraz(M2);break;
		case 3: printf("La solucion Dinamica es: \n");
				solDinamica(M2);break;
		case 4: break;		
		default: printf("Opcion incorrecta \n");break;
	}
	}	
	system("pause");
	return EXIT_SUCCESS;
}
