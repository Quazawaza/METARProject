#include <iostream>
#include <fstream>

using namespace std;

void Starter(char &inputaction)
{
    cout << "Tomasz Ciurkowski 203191" << endl;
    cout << "Wybierz co chcesz zrobic: " << endl;
    cout << "'q' - wyjscie z programu" << endl;
    cout << "'o' - pobierz dane METAR z pliku tekstowego  i zapisz przetworzone dane do innego " << endl;
    cin >> inputaction;
}

bool Action(char inputaction)
{
    if (inputaction == 'q')
    {
        return false;
    }
    if (inputaction == 'o')
    {
        return true;
    }
    return false;
}

void Input(char inputfile[105], char outputfile[105])
{
    cin.ignore();
    cout <<"Prosze wprowadzic nazwe wejsciowego pliku txt z danymi METAR (dlugosc nazwy nie powinna przekraczac 100 znakow)" << endl;
    cin.getline(inputfile,105);
    cout << "Prosze wprowadzic nazwe pliku wyjsciowego txt (nazwa pliku nie powinna przekraczac 100 znakow)" << endl;
    cin.getline(outputfile, 105);
}

void ProcessFile(char inputfile[105]) {
    fstream infile;
    infile.open(inputfile,ios::in);
    if (!infile) {
        cout << "blad";
    }
    const int linelenght = 128;
    char *line = new char[linelenght];

    while (infile.getline(line, linelenght)) {
        char *ptr = line;
         while (*ptr != '\0') {
            cout << *ptr;
            ++ptr;
        }
        cout << endl;
    }

    if (infile.bad()) {
        cout << "blad";
    }
    delete[] line;
    infile.close();

}

int main() {
    char inputaction;
    Starter(inputaction);
    if (Action(inputaction)) {
        char inputfile[105];
        char outputfile[105];
        Input(inputfile, outputfile);
        ProcessFile(inputfile);
        return 0;
    }
    else {
        return 1;
    }

}