/*
 *======================
 *   Sudoku Generator
 *======================
 *

*/

#include<iostream>
#include<fstream>
using namespace std;


class SudokuFrame{
	
	int sudokuFrame[9][9]; 


	public:SudokuFrame(){
		menu();
		setZeros();
	}
	
	
	private:void menu(){
		
		cout<<"\n=======================\n";
		cout<<"    Sudoku Generator\n";
		cout<<"=======================\n\n";

		cout<<"Welcome to Sudoku Generator!\n";
		cout<<"We shall begin generating your puzzle now.\n\n";

	}


	private:void setZeros(){
		int rowIter, colIter;

		for(rowIter=0; rowIter<9; rowIter++){
			for(colIter=0; colIter<9; colIter++){
				sudokuFrame[rowIter][colIter]=0;
			}
		}
	}


	public:void setCellValue(int row, int col, int num){
		if(editableFrame[row][col]==0) sudokuFrame[row][col]=num;
	}
	

	public:int getCellValue(int row, int col){
		int cellValue=sudokuFrame[row][col];
		return cellValue;
	}
	

	public:void clearFrameFrom(int row, int col){
		int jcount=0;
		int rowIter, colIter;

		for(rowIter=row; rowIter<9; rowIter++){
			
			if(jcount==0) colIter=col;
			else colIter=0;

			for(; colIter<9; colIter++){
				if(editableFrame[rowIter][colIter]==0) sudokuFrame[rowIter][colIter]=0;
			}

			jcount++;

		}
	}

};



class Possibilities{
	
	struct node{
		int value;
		struct node* next;
	}; //The struct for the node
	
	typedef struct node* Node;

	Node head; //The head node
	Node pos; //A node iterator variable
	


	public:Possibilities(){
		head=new struct node;
		head->next=NULL;
	}
	

	public:~Possibilities(){
		destroy();
	}
	

	public:void append(int number){
		Node temp=new struct node;

		temp->value=number;
		temp->next=NULL;

		pos=head;
		while(pos!=NULL){
			if(pos->next==NULL){
				pos->next=temp;
				break;
			}
			pos=pos->next;
		}
	}
	

	public:int operator[](int index){
		int count=0;
		pos=head->next;
		
		while(pos!=NULL){
			if(count==index)
				return pos->value;
			pos=pos->next;
			count++;
		}

		return -1;
	}


	public:void print(){
		pos=head->next;
		while(pos!=NULL){
			cout<<pos->value<<",";
			pos=pos->next;
		}
		cout<<"\b";
	}


	public:int length(){
		pos=head->next;
		int count=0;

		while(pos!=NULL){
			count++;
			pos=pos->next;
		}
		
		return count;
	}
	

	public:void copy(Possibilities possibilities){ 
		int oldLength=possibilities.length();
		int iter=0;
		
		pos=head;
		for(iter=0; iter<oldLength; iter++){
			Node temp=new struct node;

			temp->next=NULL;
			temp->value=possibilities[iter];

			pos->next=temp;
			pos=pos->next;
		}
	}
	

	private:void destroy(){
		Node temp;
		pos=head;
		while(pos!=NULL){
			temp=pos;
			pos=pos->next;
			delete temp;
		}
	}

};



class SudokuSolver{
	
	int recursiveCount; //Stats variable
	SudokuFrame frame; //The frame object
	

	public:SudokuSolver(){
		recursiveCount=0;

		cout<<"\nCalculating possibilities...\n";
		cout<<"Backtracking across puzzle....\n";
		cout<<"Validating cells and values...\n\n";
		
		solve();
		cout<<"QED. Your puzzle has been solved!\n\n";
		displayFrame();

		cout<<"\n\n";
	}
	

	private:bool cellValueValid(int row, int col, int currentValue){
		int rowIter, colIter;

		
		for(rowIter=0; rowIter<9; rowIter++){
			if(rowIter!=row){
				int comparingValue=frame.getCellValue(rowIter,col);
				if(comparingValue==currentValue) return false;
			}
		}

		//Checking if value exists in same row
		for(colIter=0; colIter<9; colIter++){
			if(colIter!=col){
				int comparingValue=frame.getCellValue(row,colIter);
				if(comparingValue==currentValue) return false;
			}
		}

		//Checking if value exists in the same 3x3 square block
		if(ThreeByThreeGridValid(row,col,currentValue)==false) return false;

		return true;
	}
	
	
	private:bool ThreeByThreeGridValid(int row, int col, int currentValue){
		int rowStart=(row/3)*3;
		int rowEnd=(rowStart+2);

		int colStart=(col/3)*3;
		int colEnd=(colStart+2);
		
		int rowIter, colIter;

		for(rowIter=rowStart; rowIter<=rowEnd; rowIter++){
			for(colIter=colStart; colIter<=colEnd; colIter++){
				if(frame.getCellValue(rowIter,colIter)==currentValue) return false;
			}
		}

		return true;	
	}
	

	private:Possibilities getCellPossibilities(int row, int col){
		int iter=0;

		Possibilities possibilities;

		for(iter=1; iter<=9; iter++){
			if(cellValueValid(row,col,iter)==true)
				possibilities.append(iter);
		}

		return possibilities;
	}
	

	private:int singleCellSolve(int row, int col){
		
		statsIncrement(); 

		if(frame.isEditable(row,col)==true){

			Possibilities possibilities;
			possibilities.copy(getCellPossibilities(row,col));

			int posLength=possibilities.length();
			int posIter=0, newRow=row, newCol=col;

			for(posIter=0; posIter<posLength; posIter++){ 
				int possibility=possibilities[posIter];
				frame.setCellValue(row,col,possibility);
				
				
				if(col<8) newCol=col+1; 
				else if(col==8){
					if(row==8) return 1; //this means success
					newRow=row+1;
					newCol=0;
				}

				{

					if(singleCellSolve(newRow,newCol)==0){ 
						frame.clearFrameFrom(newRow,newCol);
					}
					else return 1;

				} 

			} 

			return 0;
			
		} 
		else{

			int newRow=row, newCol=col;
			
			
			if(col<8) newCol=col+1;
			else if(col==8){
				if(row==8) return 1;
				newRow=row+1;
				newCol=0;
			}

			return singleCellSolve(newRow,newCol);

		} 

	}


	private:void solve(){
		int success=singleCellSolve(0,0);
	}
	

	private:void displayFrame(){
		frame.displayFrame();
	}
	

	private:void statsIncrement(){
		recursiveCount++;
	}

	
	public:void statsPrint(){
		cout<<"\nThe singleCellSolve() function was recursively called "<<recursiveCount<<" times.\n";
	}
	
};

int main(){
	SudokuSolver ss;
	return 0;
}
