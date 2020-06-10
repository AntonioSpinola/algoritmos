
#include <cstdio>
#include <utility> 
#include <cfloat>
#include <stack>
#include <set>
#include <iostream>
using namespace std;

//#define Fila 9 
//#define Columna 10 

#define Fila 15
#define Columna 18

typedef pair<int, int> Pair;
typedef pair<double, pair<int, int>> pPair;

// Estrucutra para manejar los parametros
struct cell
{
   
    int parent_i, parent_j;
    // f = g + h 
    double f, g, h;
};



//Checar si la celda es valida o no 
bool isValid(int row, int col)
{
   //regresa verdadero si estan los valores de filas y columnas en el rango
    return (row >= 0) && (row < Fila) &&
        (col >= 0) && (col < Columna);
}

//Checar si la celda esta bloqueada o no
bool isUnBlocked(int grid[][Columna], int row, int col)
{
    // Returns true if the cell is not blocked else false 
    if (grid[row][col] == 1)
        return (true);
    else
        return (false);
}

// Para checar si se alcanzo la ubicacion
bool isDestination(int row, int col, Pair dest)
{
    if (row == dest.first && col == dest.second)
        return (true);
    else
        return (false);
}

//Calcular la distancia
double calculateHValue(int row, int col, Pair dest)
{
    
    return ((double)sqrt((row - dest.first) * (row - dest.first)
        + (col - dest.second) * (col - dest.second)));
}

//Funcion para encontrar el destino desde el inicio
void tracePath(cell cellDetails[][Columna], Pair dest)
{

    printf("\n El Camino es el siguiente: ");
    int row = dest.first;
    int col = dest.second;

    stack<Pair> Path;

    while (!(cellDetails[row][col].parent_i == row
        && cellDetails[row][col].parent_j == col))
    {
        Path.push(make_pair(row, col));
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }

    Path.push(make_pair(row, col));
    while (!Path.empty())
    {
        pair<int, int> p = Path.top();
        Path.pop();
        printf("-> (%d,%d) ", p.first, p.second);
    }

    return;
}

// Encontrar el camino mas corto de acuerdo al algoritmo 'A*'
void aStarSearch(int grid[][Columna], Pair src, Pair dest)
{
    
    if (isValid(src.first, src.second) == false)
    {
        printf("El inicio es invalido, intentelo de nuevo\n");
        return;
    }

  
    if (isValid(dest.first, dest.second) == false)
    {
        printf("El destino es invalido, intentelo de nuevo\n");
        return;
    }

    
    if (isUnBlocked(grid, src.first, src.second) == false ||
        isUnBlocked(grid, dest.first, dest.second) == false)
    {
        printf("El inicio o destino esta bloqueado, por favor desbloqueelo o intentelo de nuevo\n");
        return;
    }

    
    if (isDestination(src.first, src.second, dest) == true)
    {
        printf("Se encuentra en el destino\n");
        return;
    }

    
    bool closedList[Fila][Columna];
    memset(closedList, false, sizeof(closedList));

    //Declaracion de arreglo 2d para su interpretacion
    cell cellDetails[Fila][Columna];

    int i, j;

    for (i = 0; i < Fila; i++)
    {
        for (j = 0; j < Columna; j++)
        {
            cellDetails[i][j].f = FLT_MAX;
            cellDetails[i][j].g = FLT_MAX;
            cellDetails[i][j].h = FLT_MAX;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
        }
    }

    // Aqui se inicializan los parametros 
    i = src.first, j = src.second;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;

   //creacion de la lista para agregar sus valores
    set<pPair> openList;
    openList.insert(make_pair(0.0, make_pair(i, j)));

    //lo inicializamos ya
    //que al comienzo no se encuentra el destino
    bool foundDest = false;

    while (!openList.empty())
    {
        pPair p = *openList.begin();

       
        openList.erase(openList.begin());

        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;

       //aqui es necesario crear las opciones para los diferentes caminos que puede tomar
        double gNew, hNew, fNew;

        //-----------Norte------------ 

       
        if (isValid(i - 1, j) == true)
        {
           
            if (isDestination(i - 1, j, dest) == true)
            {
                // Encuentra el camino y lo hace valido
                cellDetails[i - 1][j].parent_i = i;
                cellDetails[i - 1][j].parent_j = j;
                printf("Se encontro el camino con exito\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
         
            // Si esta bloqueado  
            else if (closedList[i - 1][j] == false &&
                isUnBlocked(grid, i - 1, j) == true)
            {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i - 1, j, dest);
                fNew = gNew + hNew;

                if (cellDetails[i - 1][j].f == FLT_MAX ||
                    cellDetails[i - 1][j].f > fNew)
                {
                    openList.insert(make_pair(fNew,
                        make_pair(i - 1, j)));

                    //Aqui actualizamos las celdas 
                    cellDetails[i - 1][j].f = fNew;
                    cellDetails[i - 1][j].g = gNew;
                    cellDetails[i - 1][j].h = hNew;
                    cellDetails[i - 1][j].parent_i = i;
                    cellDetails[i - 1][j].parent_j = j;
                }
            }
        }

        //----------- Sur ------------ 

        
        if (isValid(i + 1, j) == true)
        {  
            if (isDestination(i + 1, j, dest) == true)
            {
                cellDetails[i + 1][j].parent_i = i;
                cellDetails[i + 1][j].parent_j = j;
                printf("Se encontro el camino con exito\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
            
            else if (closedList[i + 1][j] == false &&
                isUnBlocked(grid, i + 1, j) == true)
            {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i + 1, j, dest);
                fNew = gNew + hNew;

                if (cellDetails[i + 1][j].f == FLT_MAX ||
                    cellDetails[i + 1][j].f > fNew)
                {
                    openList.insert(make_pair(fNew, make_pair(i + 1, j)));          
                    cellDetails[i + 1][j].f = fNew;
                    cellDetails[i + 1][j].g = gNew;
                    cellDetails[i + 1][j].h = hNew;
                    cellDetails[i + 1][j].parent_i = i;
                    cellDetails[i + 1][j].parent_j = j;
                }
            }
        }

        //----------- Este------------ 

       
        if (isValid(i, j + 1) == true)
        {
            if (isDestination(i, j + 1, dest) == true)
            {
                cellDetails[i][j + 1].parent_i = i;
                cellDetails[i][j + 1].parent_j = j;
                printf("Se encontro el camino con exito\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
            else if (closedList[i][j + 1] == false &&
                isUnBlocked(grid, i, j + 1) == true)
            {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j + 1, dest);
                fNew = gNew + hNew;

                if (cellDetails[i][j + 1].f == FLT_MAX ||
                    cellDetails[i][j + 1].f > fNew)
                {
                    openList.insert(make_pair(fNew,
                        make_pair(i, j + 1)));
                    cellDetails[i][j + 1].f = fNew;
                    cellDetails[i][j + 1].g = gNew;
                    cellDetails[i][j + 1].h = hNew;
                    cellDetails[i][j + 1].parent_i = i;
                    cellDetails[i][j + 1].parent_j = j;
                }
            }
        }

        //----------- Oeste------------ 

        
        if (isValid(i, j - 1) == true)
        {
            if (isDestination(i, j - 1, dest) == true)
            {
                cellDetails[i][j - 1].parent_i = i;
                cellDetails[i][j - 1].parent_j = j;
                printf("Se encontro el camino con exito\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
            else if (closedList[i][j - 1] == false &&
                isUnBlocked(grid, i, j - 1) == true)
            {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j - 1, dest);
                fNew = gNew + hNew;

                if (cellDetails[i][j - 1].f == FLT_MAX ||
                    cellDetails[i][j - 1].f > fNew)
                {
                    openList.insert(make_pair(fNew,
                        make_pair(i, j - 1)));
                    cellDetails[i][j - 1].f = fNew;
                    cellDetails[i][j - 1].g = gNew;
                    cellDetails[i][j - 1].h = hNew;
                    cellDetails[i][j - 1].parent_i = i;
                    cellDetails[i][j - 1].parent_j = j;
                }
            }
        }

        //----------- Noreste------------ 

        
        if (isValid(i - 1, j + 1) == true)
        {
            if (isDestination(i - 1, j + 1, dest) == true)
            {
                cellDetails[i - 1][j + 1].parent_i = i;
                cellDetails[i - 1][j + 1].parent_j = j;
                printf("Se encontro el camino con exito\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

            else if (closedList[i - 1][j + 1] == false &&
                isUnBlocked(grid, i - 1, j + 1) == true)
            {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i - 1, j + 1, dest);
                fNew = gNew + hNew;

                if (cellDetails[i - 1][j + 1].f == FLT_MAX ||
                    cellDetails[i - 1][j + 1].f > fNew)
                {
                    openList.insert(make_pair(fNew,
                        make_pair(i - 1, j + 1)));
                    cellDetails[i - 1][j + 1].f = fNew;
                    cellDetails[i - 1][j + 1].g = gNew;
                    cellDetails[i - 1][j + 1].h = hNew;
                    cellDetails[i - 1][j + 1].parent_i = i;
                    cellDetails[i - 1][j + 1].parent_j = j;
                }
            }
        }

        //-----------Noroeste------------ 

        if (isValid(i - 1, j - 1) == true)
        {
            if (isDestination(i - 1, j - 1, dest) == true)
            {
                cellDetails[i - 1][j - 1].parent_i = i;
                cellDetails[i - 1][j - 1].parent_j = j;
                printf("Se encontro el camino con exito\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

            else if (closedList[i - 1][j - 1] == false &&
                isUnBlocked(grid, i - 1, j - 1) == true)
            {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i - 1, j - 1, dest);
                fNew = gNew + hNew;
                if (cellDetails[i - 1][j - 1].f == FLT_MAX ||
                    cellDetails[i - 1][j - 1].f > fNew)
                {
                    openList.insert(make_pair(fNew, make_pair(i - 1, j - 1)));
                    cellDetails[i - 1][j - 1].f = fNew;
                    cellDetails[i - 1][j - 1].g = gNew;
                    cellDetails[i - 1][j - 1].h = hNew;
                    cellDetails[i - 1][j - 1].parent_i = i;
                    cellDetails[i - 1][j - 1].parent_j = j;
                }
            }
        }

        //----------- Sureste------------ 

        
        if (isValid(i + 1, j + 1) == true)
        {
            if (isDestination(i + 1, j + 1, dest) == true)
            {
                cellDetails[i + 1][j + 1].parent_i = i;
                cellDetails[i + 1][j + 1].parent_j = j;
                printf("Se encontro el camino con exito \n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }

         
            else if (closedList[i + 1][j + 1] == false &&
                isUnBlocked(grid, i + 1, j + 1) == true)
            {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i + 1, j + 1, dest);
                fNew = gNew + hNew;

             
                if (cellDetails[i + 1][j + 1].f == FLT_MAX ||
                    cellDetails[i + 1][j + 1].f > fNew)
                {
                    openList.insert(make_pair(fNew,
                        make_pair(i + 1, j + 1)));
                    cellDetails[i + 1][j + 1].f = fNew;
                    cellDetails[i + 1][j + 1].g = gNew;
                    cellDetails[i + 1][j + 1].h = hNew;
                    cellDetails[i + 1][j + 1].parent_i = i;
                    cellDetails[i + 1][j + 1].parent_j = j;
                }
            }
        }

        //-----------Suroeste ------------ 


        if (isValid(i + 1, j - 1) == true)
        {
            if (isDestination(i + 1, j - 1, dest) == true)
            {
             
                cellDetails[i + 1][j - 1].parent_i = i;
                cellDetails[i + 1][j - 1].parent_j = j;
                printf("Se encontro camino con exito:\n");
                tracePath(cellDetails, dest);
                foundDest = true;
                return;
            }
            else if (closedList[i + 1][j - 1] == false &&
                isUnBlocked(grid, i + 1, j - 1) == true)
            {
                gNew = cellDetails[i][j].g + 1.414;
                hNew = calculateHValue(i + 1, j - 1, dest);
                fNew = gNew + hNew;

                if (cellDetails[i + 1][j - 1].f == FLT_MAX ||
                    cellDetails[i + 1][j - 1].f > fNew)
                {
                    openList.insert(make_pair(fNew,
                        make_pair(i + 1, j - 1)));

                   
                    cellDetails[i + 1][j - 1].f = fNew;
                    cellDetails[i + 1][j - 1].g = gNew;
                    cellDetails[i + 1][j - 1].h = hNew;
                    cellDetails[i + 1][j - 1].parent_i = i;
                    cellDetails[i + 1][j - 1].parent_j = j;
                }
            }
        }
    }

    // Cuando no se ecnuentra el destino, se blockea
    if (foundDest == false)
        printf("No se pudo encontrar el camino\n");

    return;
}


int main()
{
    // '1' = Celda no esta bloqueada
    // '0' = Celda esta bloqueada

    int grid[Fila][Columna] =
    {
        //Demo
        /*{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
        { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 },
        { 1, 1, 1, 0, 1, 1, 0, 1, 0, 1 },
        { 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
        { 1, 1, 1, 0, 1, 1, 1, 0, 1, 0 },
        { 1, 0, 1, 1, 1, 1, 0, 1, 0, 0 },
        { 1, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
        { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
        { 1, 1, 1, 0, 0, 0, 1, 0, 0, 1 }*/


        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0},
        { 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
        
    };

    int x{};//columna
    int y{};//fila
    printf("Ingresa la columna\n");
    cin >> x;
    printf("Ingresa la fila\n");
    cin >> y;

    //demo
    //Pair src = make_pair(0, 0);

     // practicamente de donde viene
    Pair src = make_pair(0, 0);

    

    // hacia donde va
    Pair dest = make_pair(x, y);

    //demo
    //Pair dest = make_pair(2, 4);

    aStarSearch(grid, src, dest);

    return(0);
}