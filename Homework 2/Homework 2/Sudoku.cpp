// Sudoku.cpp : Basic class for holding a Sudoku board, reading a board from files, a writing a board to the screen
//

#include<iostream>
#include<fstream>
#include<sstream>

#include<string>
#include<math.h>

using namespace std;

class Board {
	int dim;
	int ** cells;
	long totalChecks;
public:
	Board (int);
	~Board();
	string toString();
	void set_square_value(int,int,int);
	int Board::get_square_value(int,int);
	static Board * fromFile(string);
	bool checkForVictory();
	//bool SelectUnassignedCell(int&, int&);
	int get_dim() {return dim;}
};

Board::Board(int d) {
	if(d > 62)
		throw ("Dimensions must be at most 62");
	dim = d;
	cells = new int*[dim];
	for(int i=0; i<dim;i++) {
		cells[i] = new int[dim];
		for(int j=0; j<dim;j++)
			cells[i][j] = 0;
	}
	totalChecks = 0;
}

Board::~Board() {
	/*for(int i=0; i<dim;i++) {
		delete [] cells[i];
	}
	delete [] cells;*/
}

string Board::toString() {
	stringstream s;
	for(int i=0; i<dim;i++) {
		for(int j=0; j<dim; j++) {
			if(cells[i][j]==0)
				s << '_';
			else
				s << cells[i][j];
		}
		s << endl;
	}
	return s.str();
}

void Board::set_square_value(int row, int col, int val) {
	cells[row-1][col-1] = val;
}

int Board::get_square_value(int row, int col) {
	return cells[row-1][col-1];
}

Board * Board::fromFile(string inFileName) {
  string line;
  ifstream inFile (inFileName.c_str());
  Board * out = nullptr;
  if (inFile.is_open()) {
	  getline (inFile,line);
	  int d = atoi(line.c_str());
	  out = new Board(d);
	  getline (inFile, line);
	  int numVals = atoi(line.c_str());
	  for(int i=0; i<numVals;i++) {
		string field;
		getline (inFile,field, '\t');
		int row = atoi(field.c_str());
		getline (inFile,field, '\t');
		int col = atoi(field.c_str());
		getline (inFile,field);
		int val = atoi(field.c_str());
		out->set_square_value(row, col, val);
	  }
  }
  inFile.close();
  return out;
}

bool Board::checkForVictory() {
	unsigned long victory = 0;
	//optimization: check if it's filled:
	for(int i=1; i<dim+1;i++)
		for(int j=1;j<dim+1;j++)
			if(this->get_square_value(i,j)==0)
				return false;
	for(int i=1; i<dim+1; i++) 
		victory += 1 << i;
	//check rows and columns:
	for(int i=1; i<dim+1;i++) {
		unsigned long rowTotal = 0;
		unsigned long columnTotal = 0;
		for(int j=1; j<dim+1; j++) {
			rowTotal += 1 << this->get_square_value(i, j);
			columnTotal += 1 << this->get_square_value(j, i);
		}
		if(rowTotal!=victory||columnTotal!=victory)
			return false;
	}
	int dimsqrt = (int)(sqrt((double)dim));
	//check little squares:
	cout << "checking little squares" << endl;
	for(int i=0;i<dimsqrt;i++) {
		for(int j=0;j<dimsqrt;j++) {
			unsigned long squareTotal = 0;
			for(int k=1; k<dimsqrt+1;k++) {
				for(int m=1; m<dimsqrt+1;m++) {
					squareTotal += 1 << this->get_square_value(i*dimsqrt+k, j*dimsqrt+m);
					cout << this->get_square_value(i*dimsqrt+k, j*dimsqrt+m);
				}
				cout << endl;
			}
			if(squareTotal != victory)
				return false;
		}
	}
	return true;
}

//bool Board::SelectUnassignedCell(int &row, int &col)
//{
//	for(int i=1; i<=this->get_dim(); i++)
//		for(int j=1; j<=this->get_dim(); j++)
//		{
//			//int sq = this->get_square_value(i,j);
//			if (this->get_square_value(i,j)==0)
//			{
//				row=i;
//				col=j;
//				return true;
//			}
//		}
//	return false;
//}


void testBasics() {
	Board * b = new Board(4);
	b->set_square_value(1, 1, 1);
	b->set_square_value(1, 2, 2);
	b->set_square_value(1, 3, 3);
	b->set_square_value(1, 4, 4);
	b->set_square_value(2, 1, 3);
	b->set_square_value(2, 2, 4);
	b->set_square_value(2, 3, 1);
	b->set_square_value(2, 4, 2);
	b->set_square_value(3, 1, 4);
	b->set_square_value(3, 2, 3);
	b->set_square_value(3, 3, 2);
	b->set_square_value(3, 4, 1);
	b->set_square_value(4, 1, 2);
	b->set_square_value(4, 2, 1);
	b->set_square_value(4, 3, 4);
	b->set_square_value(4, 4, 3);
	cout << b->toString();
	cout << b->checkForVictory();
	char a;
	cin >> a;
}

//prototypes
string Backtracking(Board);
string RecursiveBackTracking(Board, Board);
bool SelectUnassignedCell(Board, int &, int &);
bool Consistent(int, int, int, Board);
string ForwardChecking(Board);
string RecursiveForwardChecking(Board, Board);
bool FCConsistent(int, int, int, Board);
bool HasRemainingValues(int, int, Board)

long checks=0;

int main(int argc, char* argv[])
{
	checks=0;
	Board * b = Board::fromFile("4x4.txt");
	//cout<<"Solution:"<<endl<<Backtracking(*b)<<endl;
	cout<<"Solution:"<<endl<<ForwardChecking(*b)<<endl;
	cout<<"Consistency checks: "<<checks<<endl;
	char a;
	cin>>a;
	return 0;
}


string Backtracking(Board c)
{
	return RecursiveBackTracking(c, c);
}

string RecursiveBackTracking(Board assignment, Board c)
{
	int row=1;
	int col=1;
	//cout<<assignment.toString();
	
	string failure = "Failed to solve";
	if(assignment.checkForVictory())
		return assignment.toString();
	/*if(!assignment.SelectUnassignedCell(row, col))
		return "No more unassigned cells.";*/
	if(!SelectUnassignedCell(assignment, row, col))
		return "No more unassigned cells.";
	//cout<<"Selected cell: row = "<<row<<" col = "<<col<<endl;
	//cout<<"here now\n"<<c.toString();
	for(int i=1; i<=assignment.get_dim(); i++)//check all domain values of chosen cell
	{
		//cout<<"Checking value: "<<i<<endl;
		if(Consistent(row, col, i, assignment))
		{
			assignment.set_square_value(row, col, i);//add to assignment
			cout<<checks<<endl;
			string result = RecursiveBackTracking(assignment, c);
			if (result!=failure)
				return assignment.toString();
			assignment.set_square_value(row, col, 0);//remove from assignment
		}
		/*else
			cout<<"Value: "<<i<<" not consistent.\n";*/
	}
	return failure;
}

bool SelectUnassignedCell(Board assignment, int &row, int &col)
{
	for(int i=1; i<=assignment.get_dim(); i++)
		for(int j=1; j<=assignment.get_dim(); j++)
		{
			int sq = assignment.get_square_value(i,j);
			if (sq==0)
			{
				row=i;
				col=j;
				return true;
			}
		}
	return false;
}

bool Consistent(int row, int col, int i, Board assignment)
{
		
	//cout<<"here\n"<<assignment.toString();
	checks++;
	//check row
	//cout<<"checking row\n";
	for(int j=1; j<=assignment.get_dim(); j++)
		if(assignment.get_square_value(row, j)==i)
			return false;
	//check column
		//cout<<"checking column\n";
	for(int j=1; j<=assignment.get_dim(); j++)
		if(assignment.get_square_value(j, col)==i)
			return false;
	//check small square
		//cout<<"checking square\n";
	double d=sqrt(assignment.get_dim());
	for(int k=1+ceil((double)row/d-1)*d; k<1+ceil((double)row/d-1)*d+d; k++)
		for(int j=1+ceil((double)col/d-1)*d; j<1+ceil((double)col/d-1)*d+d; j++)
		{
			//cout<<"checking position: "<<k<<", "<<j<<endl;
			if (!(k==row && j==col))
				if(assignment.get_square_value(k, j)==i)
					return false;
		}
	return true;
}

string ForwardChecking(Board c)
{
	return RecursiveForwardChecking(c, c);
}

string RecursiveForwardChecking(Board assignment, Board c)
{
	int row=1;
	int col=1;
	//cout<<assignment.toString();
	
	string failure = "Failed to solve";
	if(assignment.checkForVictory())
		return assignment.toString();
	/*if(!assignment.SelectUnassignedCell(row, col))
		return "No more unassigned cells.";*/
	if(!SelectUnassignedCell(assignment, row, col))
		return "No more unassigned cells.";
	//cout<<"Selected cell: row = "<<row<<" col = "<<col<<endl;
	//cout<<"here now\n"<<c.toString();
	for(int i=1; i<=assignment.get_dim(); i++)//check all domain values of chosen cell
	{
		//cout<<"Checking value: "<<i<<endl;
		if(FCConsistent(row, col, i, assignment))
		{
			assignment.set_square_value(row, col, i);//add to assignment
			cout<<checks<<endl;
			string result = RecursiveBackTracking(assignment, c);
			if (result!=failure)
				return assignment.toString();
			assignment.set_square_value(row, col, 0);//remove from assignment
		}
		/*else
			cout<<"Value: "<<i<<" not consistent.\n";*/
	}
	return failure;
}

bool FCConsistent(int row, int col, int i, Board assignment)
{
		
	//cout<<"here\n"<<assignment.toString();
	checks++;
	//check row
	//cout<<"checking row\n";
	for(int j=1; j<=assignment.get_dim(); j++)
		if(assignment.get_square_value(row, j)==i)
			return false;
	//check column
		//cout<<"checking column\n";
	for(int j=1; j<=assignment.get_dim(); j++)
		if(assignment.get_square_value(j, col)==i)
			return false;
	//check small square
		//cout<<"checking square\n";
	double d=sqrt(assignment.get_dim());
	for(int k=1+ceil((double)row/d-1)*d; k<1+ceil((double)row/d-1)*d+d; k++)
		for(int j=1+ceil((double)col/d-1)*d; j<1+ceil((double)col/d-1)*d+d; j++)
		{
			//cout<<"checking position: "<<k<<", "<<j<<endl;
			if (!(k==row && j==col))
				if(assignment.get_square_value(k, j)==i)
					return false;
		}
	return true;
}

bool HasRemainingValues(int row, int col, Board assignment)
{
	int numRemaining=assignment.get_dim();
	for(int i=1; i<=assignment.get_dim(); i++)
	{
		//check row
		for(int j=1; j<=assignment.get_dim(); j++)
			if(assignment.get_square_value(row, j)==i)
				numRemaining--;
		//check column
		for(int j=1; j<=assignment.get_dim(); j++)
			if(assignment.get_square_value(j, col)==i)
				numRemaining--;
		//check small square
		double d=sqrt(assignment.get_dim());
		for(int k=1+ceil((double)row/d-1)*d; k<1+ceil((double)row/d-1)*d+d; k++)
			for(int j=1+ceil((double)col/d-1)*d; j<1+ceil((double)col/d-1)*d+d; j++)
			{
				//cout<<"checking position: "<<k<<", "<<j<<endl;
				if (!(k==row && j==col))
					if(assignment.get_square_value(k, j)==i)
						return false;
			}
	}
	return true
}