char * createBoard(std::size_t xdim, std::size_t ydim);
void computeNeighbors(char * board, std::size_t xdim, std::size_t ydim);
void hideBoard(char * board, std::size_t xdim, std::size_t ydim);
void cleanBoard(char * board);
void printBoard(char * board, std::size_t xdim, std::size_t ydim);
int reveal(char * board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc);
int mark(char * board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc);
bool isGameWon(char * board, std::size_t xdim, std::size_t ydim);

int hidden { 0b00100000 }; // hidden
int marked { 0b00110000 }; // marked ( hidden + mark)

char *createBoard(std::size_t xdim, std::size_t ydim){
    char *gameboard = new char [xdim * ydim] ;

    for ( unsigned long i{0}; i < xdim*ydim ; i++) {
        gameboard[i] = 0x00 ;
    }

    return gameboard;
}

void hideBoard(char *board, std::size_t xdim, std::size_t ydim){    
    for ( unsigned long i{0}; i < xdim*ydim ; i++){
        if ( hidden != (board[i]&hidden) )
            board[i] |= hidden;
    }
}

void cleanBoard(char *board){
    delete[] board;
    board = {nullptr};
}

void printBoard(char *board, std::size_t xdim, std::size_t ydim){
    for ( unsigned long i{0}; i < xdim*ydim ; i++){
        
        if ( i % xdim == 0 && i != 0)
            std::cout << "\n" << std::endl;
        //                ^ personmal prefrence NOT INCLUDE IN SUBMISSION

        std::cout << "\t" << std::endl;
        //  ^ personmal prefrence NOT INCLUDE IN SUBMISSION

        if ( marked == (board[i]&marked) )  
            std::cout << "M" ;
        
        else if ( hidden == ( board[i]&hidden ) ) 
            std::cout << "O" ;

        else if ( 0x00 ==  (board[i]) )
            std::cout << "-";
        // personal prefrence. NOT INCLUDE IN SUBMISSION ^^

        else{   
            char a = board[i]  + 48;              
            std::cout << (a) ;
        }
    }
    std::cout << std::endl;                               
}

int mark(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc,std::size_t yloc){
    int element = yloc*xdim + xloc;

    if ( marked == (board[element]&marked) ||  hidden == (board[element]&hidden) ){
        board[element] ^= 0b00010000;
        return 0;
    }

    else if ( 0x00 == ( board[element]&hidden ) )
        return 2;

}

void computeNeighbors(char *board, std::size_t xdim, std::size_t ydim){
    int xLoc, yLoc;

    for ( unsigned long i{}; i < xdim*ydim; ++i){
        if ( (board[i]&0x09) == 0x09 ){
            xLoc = i % xdim;
            yLoc = i / xdim;

            for ( int yShift{-1}; yShift<2; ++yShift ){
                if ( 0 <= yLoc + yShift && yLoc + yShift < ydim){

                    for ( int xShift{-1}; xShift<2; ++xShift ){
                        if ( 0 <= xLoc + xShift && xLoc + xShift < xdim ){
                            
                            if ( yShift*xdim + xShift != 0 && (board[i + yShift*xdim + xShift]&0x09) != 0x09) {
                                board[i + yShift*xdim + xShift] += 0b00000001;
                            }                          
                        }                
                    }
                }
            }
        }
    }
}

int reveal(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc,std::size_t yloc){
    int element = yloc*xdim + xloc;

    if ( marked == (board[element]&marked) )
        return 1;
    
    if ( hidden !=  (board[element]&hidden) )
        return 2;

    else 
        board[element] ^= hidden;

    if (  (board[element]&0x09) == 0x09 )      // goose
        return 9;
    
    if (  0x00 ==  (board[element]) ){
        for ( int yShift{-1}; yShift<2; ++yShift ){
            
            if ( 0 <= yloc + yShift && yloc + yShift < ydim){
                for ( int xShift{-1}; xShift<2; ++xShift ){
                    
                    if ( 0 <= xloc + xShift && xloc + xShift < xdim ){
                        if ( yShift*xdim + xShift != 0 && (board[element + yShift*xdim + xShift]&0x09) != 0x09) {
                            if ( hidden ==  (board[element + yShift*xdim + xShift]&hidden)  &&  marked != (board[element + yShift*xdim + xShift]&marked) ){
                                
                                if (  0x00 ==  (board[element + yShift*xdim + xShift]^hidden) )
                                    reveal( board, xdim, ydim,  xloc+xShift, yloc+yShift);
                                 //only for the real minesweeper game ^
                            
                                else
                                    board[element + yShift*xdim + xShift] ^= hidden;
                            }
                        }                          
                    }                
                }
            }
        }
    }
    return 0;
}

bool isGameWon(char *board, std::size_t xdim, std::size_t ydim){
    for ( int i{}; i<xdim*ydim; ++i){
        if ( (board[i]&hidden) == hidden && (board[i]&0x09) != 0x09){
            return false;
        }
    }
    return true;
}
