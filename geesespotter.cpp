#include "geesespotter_lib.h"
#include <iostream>

// test: g++ -o main geesespotter.cpp geesespotter_lib.cpp -std=c++11

char *createBoard(std::size_t xdim, std::size_t ydim){
    char *array{ new char[100]{} };
    for (int begin = 0; begin < xdim*ydim; begin++){
        array [begin] = 0;    
    }

return array;

}

void cleanBoard(char *board){
    delete[] board;
    board = nullptr;
}

void printBoard(char *board, std::size_t xdim, std::size_t ydim){
    for (int index = 0; index < xdim*ydim; index++){
        
        //If field is marked or hidden+marked
        if ((board[index] & markedBit()) != 0){
            std::cout << "M";
        }

        //If field is hidden
        else if ((board[index] & hiddenBit()) != 0){
            std::cout << "*";
        }

        //If field is a number
        else {
            //valueMask gives bits of number and printing out the number using int
            int number = board[index] & valueMask();
            std::cout << number;
        }

        //If index is at the end of xdim
        if (((index+1) % xdim) == 0){
            std::cout << "\n";
        }
            
        
    }

}

void hideBoard(char *board, std::size_t xdim, std::size_t ydim){
    
    for (int index = 0; index < xdim*ydim; index++ ){
        board[index] |= hiddenBit();
    }
    
}

int mark(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc,
std::size_t yloc){
    // 4,2 index 14
    // 5,3 index

    int index = xloc + yloc*xdim;
    
    //If field is revealed
    if (((board[index] & hiddenBit()) == 0) && ((board[index] & markedBit())) == 0){
        return 2;
    } 

    //Every other case, toggle the field
    else{
        board[index] ^= markedBit();
        return 0;
    }
}

void computeNeighbors(char *board, std::size_t xdim, std::size_t ydim){
    //Iterate through array
    for (int index = 0; index < xdim*ydim; index++ ){

        //change binary to int
        int neighbours =  board[index] & valueMask();
        
        //if the index is not 9, check neighbours
        if (neighbours != 9) {
            while (true){
                int neighbours2 = 0;
                int increment = 0;

                //Checking row above (account for edges and corners)
                if ((index - xdim) >= 0){
                    if ((neighbours2 = board[index - xdim] & valueMask()) == 9){
                        increment++;
                    }

                    //Left, checking if index is at left column
                    if ((index % xdim) != 0){
                        if ((neighbours2 = board[index - xdim - 1] & valueMask()) == 9){
                        increment++;
                        }
                    }
                    
                    //Right, checking if index is at right column
                    if (((index+1) % xdim) != 0){
                        if ((neighbours2 = board[index - xdim + 1] & valueMask()) == 9){
                        increment++;
                        }
                    }
                }

                //Checking adjacent fields, 

                //Left, and if index is at left edge
                if (((index % xdim) != 0)){
                    if ((neighbours2 = board[index-1] & valueMask()) == 9){
                        increment++;
                    }
                }
                
                //Right, not at right edge
                if (((index+1) % xdim) != 0){
                    if ((neighbours2 = board[index+1] & valueMask()) == 9){
                        increment++;
                    }
                }

            
                //Checking row below (account for edges and corners)
                if ((index + xdim) < xdim*ydim ){

                    //Directly Below
                    if ((neighbours2 = board[index + xdim] & valueMask()) == 9){
                        increment++;
                    }

                    //Left, checking if index is at left column
                    if ((index % xdim) != 0){
                        if ((neighbours2 = board[index + xdim - 1] & valueMask()) == 9){
                        increment++;
                        }
                    }
                    
                    //Right, cheching if index is at right column
                    if ((((index+1) % xdim) != 0)){
                        if ((neighbours2 = board[index + xdim + 1] & valueMask()) == 9){
                        increment++;
                        }
                    }
                }

                board[index] |= increment;
                break;
            }           
        }
    }
}


int reveal(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc,
std::size_t yloc){
    
    int index = xloc + yloc*xdim;
    int number = 0;

    //if field is hidden and unmarked
    if (((board[index] & hiddenBit()) != 0) && ((board[index] & markedBit()) == 0)){
        //set hidden bit to 0, using XOR
        board[index] ^= hiddenBit(); 

        //If there is a goose, reveal the field
        if ((number = board[index] & valueMask()) == 9){
            return 9;
        } 

        //if the field is 0, reveal neighbouring fields
        if ((number = board[index] & valueMask()) == 0){

            //Row above and hidden
            if (((index - xdim) >= 0) && ((board[index - xdim] & hiddenBit()) != 0)){
                board[index - xdim] ^= hiddenBit(); 

                //Left, checking if index is at left column
                if (((index % xdim) != 0) && ((board[index- xdim -1] & hiddenBit()) != 0)){
                    board[index - xdim - 1] ^= hiddenBit(); 
                }
                
                //Right, checking if index is at right column
                if ((((index+1) % xdim) != 0) && ((board[index - xdim +1] & hiddenBit()) != 0)){
                    board[index - xdim + 1] ^= hiddenBit();
                }
            }

            //Checking adjacent fields, 
            //Left, and if index is at left edge
            if (((index % xdim) != 0) && ((board[index - 1] & hiddenBit()) != 0)){
                board[index - 1] ^= hiddenBit();
            }
            
            //Right, not at right edge
            if ((((index+1) % xdim) != 0) && ((board[index + 1] & hiddenBit()) != 0)){
                board[index + 1] ^= hiddenBit();
            }

            
            //Checking row below (account for edges and corners)
            if (((index + xdim) < xdim*ydim ) && ((board[index + xdim] & hiddenBit()) != 0)){

                //Directly Below
                board[index + xdim] ^= hiddenBit();

                //Left, checking if index is at left column
                if (((index % xdim) != 0) && ((board[index + xdim -1] & hiddenBit()) != 0)){
                    board[index + xdim -1] ^= hiddenBit();
                }
                
                //Right, cheching if index is at right column
                if ((((index+1) % xdim) != 0) && ((board[index + xdim +1] & hiddenBit()) != 0)){
                    board[index + xdim +1] ^= hiddenBit();
                }
            }
        }
    }

    //If field is marked
    else if ((board[index] & markedBit()) != 0){
        return 1;
    }

    //If field is revealed
    else if (((board[index] & hiddenBit()) == 0) && ((board[index] & markedBit())) == 0){
        return 2;
    }

    return 0;
}


bool isGameWon(char *board, std::size_t xdim, std::size_t ydim){
    int decimal_number= 0;
    bool gamestate;

    //Checking for 0 geese
    for (int index = 0; index < xdim*ydim; index++){
        //
        if ((decimal_number = board[index] & valueMask()) != 9){
            gamestate = true;
        }
        else{
            gamestate = false;
            break;
        }
    }

    if (gamestate == true){
        return true;
    }

    for (int index = 0; index < xdim*ydim; index++){
        //If field is hidden and not 9
        if (((board[index] & hiddenBit()) != 0) && ((decimal_number = board[index] & valueMask()) != 9)){
            return false;
        }
    }
    return true;
}