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
    cin.ignore();
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
    cout <<"Prosze wprowadzic nazwe wejsciowego pliku txt z danymi METAR (dlugosc nazwy nie powinna przekraczac 100 znakow)" << endl;
    cin.getline(inputfile,105);
    cout << "Prosze wprowadzic nazwe pliku wyjsciowego txt (nazwa pliku nie powinna przekraczac 100 znakow)" << endl;
    cin.getline(outputfile, 105);
}

void ParseMETAR(const char* line, ofstream& outfile)
{
    char ICAO[5] = "";
    char Day[3] = "";
    char UTCtime[5] = "";
    char WindDir[4] = "";
    char WindSpd[3] = "";
    char WindGst[3] = "";
    char WindDirCh[8] = "";
    char Visblty[5] = "";
    char Wthrphnmstrngh = 0;
    char Wthrphnm[3] = "";
    char Clouds[4] = "";
    char Cldhght[4] = "";
    char tmp[3] = "";
    char dewtmp[3] = "";
    char atmprs[5] = "";

    int index = 0;

    for (int i = 0; i < 4; i++)
    {
        ICAO[i] = line[index++];
    }
    
    ICAO[4] = '\0';
    index++;

    for (int i = 0; i < 2; i++)
    {
        Day[i] = line[index++];
    }
    Day[2] = '\0';
    for (int i = 0; i < 4; i++)
    {
        UTCtime[i] = line[index++];
    }
    UTCtime[4] = '\0';

    index+=2;

    for (int i = 0; i < 3; i++)
    {
        WindDir[i] = line[index++];
    }
    WindDir[3] = '\0';
    for (int i = 0; i < 2; i++)
    {
        WindSpd[i] = line[index++];
    }
    WindSpd[2] = '\0';

    if (line[index] == 'G')
    {
        index++;
        for (int i = 0; i < 2; i++)
        {
            WindGst[i] = line[index++];
        }
        WindGst[2] = '\0';
    }
    index+=3;

    if (line[index+3] == 'V')
    {
        for (int i = 0; i < 7; i++)
        {
            WindDirCh[i] = line[index++];
        }
        index++;
    }
    

    for (int i = 0; i < 4; i++)
    {
        Visblty[i] = line[index++];
    }
    Visblty[4] = '\0';

    index++;

    if (line[index] == '+' || line[index] == '-')
    {
        Wthrphnmstrngh = line[index++];
    }

    if (line[index+2] == ' ')
    {
        for (int i = 0; i < 2; i++)
        {
            Wthrphnm[i] = line[index++];
        }
        Wthrphnm[2] = '\0';
        index++;
    }

    if (line[index+2] != '/')
    {
        
        for (int i = 0; i < 3; i++)
        {
            Clouds[i] = line[index++];
        }
        Clouds[3] = '\0';

        for (int i = 0; i < 3; i++)
        {
            Cldhght[i] = line[index++];
        }
        Cldhght[3] = '\0';

        index++;        
    }

    for (int i = 0; i < 2; i++)
    {
        tmp[i] = line[index++];
    }
    tmp[2] = '\0';

    index++;

    for (int i = 0; i < 2; i++)
    {
        dewtmp[i] = line[index++];
    }
    dewtmp[2] = '\0';

    index+=2;

    for (int i = 0; i < 4; i++)
    {
        atmprs[i] = line[index++];
    }
    atmprs[4] = '\0';

    cout << "Lotnisko(ICAO): " << ICAO << endl; 
    cout << "Czas UTC: " << UTCtime[0] << UTCtime[1] << ":" << UTCtime[2] << UTCtime[3] << endl;
    cout << "Wiatr w kierunku " << WindDir << " stopni" << endl;
    cout <<  "predkosc wiatru: " << WindSpd  << " wezlow" << endl;

    if (WindGst[0] != '\0')
    {
        cout << " w porywach do: " << WindGst << "wezlow" << endl;
    }

    if (WindDirCh[0] != '\0')
    {
        cout << "zmiana kierunku wiatru miedzy: " << WindDirCh[0] << WindDirCh[1] << WindDirCh[2] << " stopni, a " << WindDirCh[4] << WindDirCh[5] << WindDirCh[6] << " stopni" << endl;
    }
    
    cout << "Widzialnosc: " << Visblty << " metrow" << endl;

    if (Wthrphnm[0] != '\0')
    {
        cout << "Zjawiska atmosferyczne: " << Wthrphnmstrngh << Wthrphnm << endl;
    }
    cout << "Zachmurzenie: " << Clouds << endl;
    cout << "podstawa chmur na wysokosci" << Cldhght << " stop" << endl;
    cout << "Temperatura: " << tmp << " stopni celsjusza"<< endl; 
    cout <<  "temperatura punktu rosy: " << dewtmp << " stopni celsjusza" << endl;
    cout << "Cisnienie atmosferyczne: " << atmprs << "hPa" << endl << endl;



    outfile << "Lotnisko(ICAO): " << ICAO << endl; 
    outfile << "Czas UTC: " << UTCtime[0] << UTCtime[1] << ":" << UTCtime[2] << UTCtime[3] << endl;
    outfile << "Wiatr w kierunku " << WindDir << " stopni" << endl;
    outfile <<  "predkosc wiatru: " << WindSpd  << " wezlow" << endl;

    if (WindGst[0] != '\0')
    {
        outfile << " w porywach do: " << WindGst << "wezlow" << endl;
    }

    if (WindDirCh[0] != '\0')
    {
        outfile << "zmiana kierunku wiatru miedzy: " << WindDirCh[0] << WindDirCh[1] << WindDirCh[2] << " stopni, a " << WindDirCh[4] << WindDirCh[5] << WindDirCh[6] << " stopni" << endl;
    }
    
    outfile << "Widzialnosc: " << Visblty << " metrow" << endl;

    if (Wthrphnm[0] != '\0')
    {
        outfile << "Zjawiska atmosferyczne: " << Wthrphnmstrngh << Wthrphnm << endl;
    }
    outfile << "Zachmurzenie: " << Clouds << endl;
    outfile << "podstawa chmur na wysokosci" << Cldhght << " stop" << endl;
    outfile << "Temperatura: " << tmp << " stopni celsjusza"<< endl; 
    outfile <<  "temperatura punktu rosy: " << dewtmp << " stopni celsjusza" << endl;
    outfile << "Cisnienie atmosferyczne: " << atmprs << "hPa" << endl << endl;


}

void FileConversion(const char inputfile[105], const char outputfile[105]) {
    ifstream infile(inputfile);
    ofstream outfile(outputfile);

    if (!infile.is_open()) 
    {
        cerr << "Nie mozna otworzyc pliku wejsciowego: " << inputfile << endl;
        return;
    }

    char line[128];
    while (infile.getline(line, 128)) 
    {
        ParseMETAR(line, outfile);
    }

    infile.close();
    outfile.close();

}
int main() {
    char inputaction;
    Starter(inputaction);
    if (Action(inputaction)) {
        char inputfile[105];
        char outputfile[105];
        Input(inputfile, outputfile);
        FileConversion(inputfile, outputfile);
        return 0;
    }
    else 
    {
        return 1;
    }

}