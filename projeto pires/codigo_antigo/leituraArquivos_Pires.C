#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main() {
  ifstream arquivo;
  arquivo.open( "numeros.txt", ios::in );
  float x, y;
  do {
    arquivo >> x >> y;
    cout << "Encontrou os números: " << x << ", " << y << " no arquivo.\n";
  } while( !arquivo.eof() );
  arquivo.close();
  arquivo.open( "numeros.txt", ios::in );
  string linha;
  cout << "\nSegunda leitura do arquivo:\n";
  while( getline( arquivo, linha ) ) {
    cout << linha << endl;
    if( linha.find( "2.5" ) != std::string::npos ) {
      cout << "  Esta linha contém 2.5.\n";
    }
  }
  arquivo.close();
  linha = "coordenadas x=5.76, y=-9.23";
  stringstream ss( linha );
  string primeiraPalavra, parteX, parteY, descarte,
    strX, strY;
  getline( ss, primeiraPalavra, ' ' );
  cout << "primeiraPalavra: " << primeiraPalavra << endl;
  getline( ss, parteX, ',' );
  cout << "parteX: " << parteX << endl;
  getline( ss, parteY );
  cout << "parteY: " << parteY << endl;
  stringstream ssParteX( parteX ), ssParteY ( parteY );
  getline( ssParteX, descarte, '=' );
  cout << "descarte: " << descarte << endl;
  getline( ssParteX, strX, ',' );
  cout << "strX: " << strX << endl;
  getline( ssParteY, descarte, '=' );
  cout << "descarte: " << descarte << endl;
  getline( ssParteY, strY );
  cout << "strY: " << strY << endl;
  x = stof( strX );
  cout << "x: " << x << endl;
  cout << "O dobro dele é " << 2 * x << endl;
  y = stof( strY );
  cout << "y: " << y << endl;
  cout << "O dobro dele é " << 2 * y << endl;
  return 0;
}
