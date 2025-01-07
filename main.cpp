#include <iostream>
#include <fstream>

using namespace std;

struct METARdata
{
    char ICAO[5] = {'\0'};
    char Day[3] = {'\0'};
    char UTCtime[5] = {'\0'};
    char WindDir[4] = {'\0'};
    char WindSpd[3] = {'\0'};
    char WindGst[3] = {'\0'};
    char WindDirCh[8] = {'\0'};
    char Visblty[6] = {'\0'};
    char Wthrphnm[3][6] = {'\0'};
    char Clouds[6][4] = {'\0'};
    char Cldhght[6][4] = {'\0'};
    int Cloudcount = 0;
    char tmp[4] = {'\0'};
    char dewtmp[4] = {'\0'};
    char atmprs[5] = {'\0'};
};

struct Airport {
    char icao[5]; 
    char name[50]; 
};

struct WeatherCode
{
    char code[3];
    char description[50];
};

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

void ICAOCode(const char* line, int& index, METARdata& data)
{
    for (int i = 0; i < 4; i++)
    {
        data.ICAO[i] = line[index++];
    }
    data.ICAO[4] = '\0';
    index++;
}

void DayNumber(const char* line, int& index, METARdata& data)
{
    for (int i = 0; i < 2; i++)
    {
        data.Day[i] = line[index++];
    }
    data.Day[2] = '\0';
}

void UTC(const char* line, int& index, METARdata& data)
{
    for (int i = 0; i < 4; i++)
    {
        data.UTCtime[i] = line[index++];
    }
    data.UTCtime[4] = '\0';
    index+=2;
}

void WindDirecton(const char* line, int& index, METARdata& data)
{
    for (int i = 0; i < 3; i++)
    {
        data.WindDir[i] = line[index++];
    }
    data.WindDir[3] = '\0';
}

void WindSpeed(const char* line, int& index, METARdata& data)
{
    for (int i = 0; i < 2; i++)
    {
        data.WindSpd[i] = line[index++];
    }
    data.WindSpd[2] = '\0';
}
void WindGust(const char* line, int& index, METARdata& data)
{
    index++;
    for (int i = 0; i < 2; i++)
    {
        data.WindGst[i] = line[index++];
    }
    data.WindGst[2] = '\0';
}
void WindDirectionChange(const char* line, int& index, METARdata& data)
{
    for (int i = 0; i < 7; i++)
    {
        data.WindDirCh[i] = line[index++];
    }
    data.WindDirCh[7] = '\0';
    index++;
}
void Visiblity(const char* line, int& index, METARdata& data)
{
    int visibilityindex = 0;
    if (line[index] == 'C')
    {
        data.Visblty[visibilityindex++] = 'C';
        index++;
    }
    for (int i = 0; i < 4; i++)
    {
        data.Visblty[visibilityindex++] = line[index++];
    }
    data.Visblty[visibilityindex++] = '\0';
    index++;
}
void WeatherPhenomena(const char* line, int& index, METARdata& data)
{
    int segmentcount = 0;

    while(line[index] != '\0' && line[index] != ' ' && segmentcount < 3 && line[index+2] != '/' && line[index+3] != '/' && !(line[index+3] >= '0' && line[index+3] <= '9')) 
    {
        if (!(line[index] == '+' || line[index] == '-')&& line[index+3] == ' ')
        {
            break;
        }
        
        int charcount = 0;
        while (line[index] != '\0' && line[index] != ' ' && charcount < 5)
        {
            data.Wthrphnm[segmentcount][charcount++] = line[index++];
        }
        data.Wthrphnm[segmentcount][charcount] = '\0';
        segmentcount++;
        while (line[index] == ' ')
        {
            index++;
        }
    }
    for (int i = segmentcount; i < 3; i++)
    {
        data.Wthrphnm[i][0] = '\0';
    }
}

void CloudsInfo(const char* line, int& index, METARdata& data)
{
    data.Cloudcount = 0;

    while (line[index] != '\0' && line[index+2] != '/' && line[index+3] != '/' && data.Cloudcount < 6 )
    {
        for (int i = 0; i < 3; i++) {
            data.Clouds[data.Cloudcount][i] = line[index++];
        }
        data.Clouds[data.Cloudcount][3] = '\0';
        if (line[index] >= '0' && line[index] <= '9')
        {
            for (int i = 0; i < 3; i++) {
                data.Cldhght[data.Cloudcount][i] = line[index++];
            }
            data.Cldhght[data.Cloudcount][3] = '\0';            
        }

        data.Cloudcount++;
        index++;
    }
    

}
void Temperature(const char* line, int& index, METARdata& data)
{
    int i = 0;
    if (line[index] == 'M')
    {
        data.tmp[i++] = '-';
        index++;
    }
    
    for (int j = 0; j < 2; j++) 
    {
        data.tmp[i++] = line[index++];
    }
    data.tmp[i] = '\0';
    index++;
}
void dewTemperature(const char* line, int& index, METARdata& data)
{
    int i = 0;
    if (line[index] == 'M')
    {
        data.dewtmp[i++] = '-';
        index++;
    }
    
    
    for (int j = 0; j < 2; j++) 
    {
        data.dewtmp[i++] = line[index++];
    }
    data.dewtmp[i] = '\0';
    index+=2;
}
void AtmosphericPressure(const char* line, int& index, METARdata& data)
{
    for (int i = 0; i < 4; i++)
    {
        data.atmprs[i] = line[index++];
    }
    data.atmprs[4] = '\0';
}


const char* TranslateICAO(const char icao[5])
{
    static const Airport airports[] = 
    {
    {"EPWA", "Warszawa-Okecie"},
    {"EPKK", "Krakow"},
    {"EPGD", "Gdansk"},
    {"EPKT", "Katowice"},
    {"EPWR", "Wroclaw"},
    {"EPMO", "Warszawa-Modlin"},
    {"EPPO", "Poznan"},
    {"EPRZ", "Rzeszow"},
    {"EPSC", "Szczecin"},
    {"EPLB", "Lublin"},
    {"EPBT", "Bydgoszcz"},
    {"EPLL", "Łodz"},
    {"EPSY", "Olsztyn"},
    {"EPRA", "Radom"},
    {"EPZG", "Zielona Gora"}
    };

    for (int i = 0; i < 15; i++)
     {
        bool match = true;
        for (int j = 2; j < 4; j++) {
            if (airports[i].icao[j] != icao[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            return airports[i].name; 
        }
    }
    return "Nieznane lotnisko"; 
}

static char* TranslateWeather(const char Wthrphnmn[3][6])
{
    static const WeatherCode intensity[] = {
        {"+", "silny "},
        {"-", "słaby "}
    };

    static const WeatherCode weather[] =
    {
        {"BR", "zamglenie"},
        {"DS", "burza pylu"},
        {"DU", "rozlegly pyl"},
        {"DZ", "mzawka"},
        {"FC", "chmura lejka"},
        {"FG", "mgla"},
        {"FU", "dym"},
        {"GR", "grad"},
        {"GS", "krupa sniezna"},
        {"HZ", "mgla"},
        {"IC", "krysztaly lodu"},
        {"PE", "granulki lodu"},
        {"PO", "wiry piasku"},
        {"PY", "pył"},
        {"RA", "deszcz"},
        {"SA", "piasek"},
        {"SG", "śnieg ziarnisty"},
        {"SN", "śnieg"},
        {"SQ", "szkwaly"},
        {"SS", "burza piaskowa"},
        {"UP", "nieznane opady"},
        {"VA", "popiół wulkaniczny"},
        {"BC", "plastry"},
        {"DR", "zamiec przyziemna"},
        {"MI", "plycizna"},
        {"PR", "czesciowy"},
        {"BL", "zawieja"},
        {"FZ", "zamrazanie"},
        {"SH", "prysznice"},
        {"TS", "burza"},
    };

    static char translation[256] = {'\0'};
    int translateindex = 0;

    for (int i = 0; i < 3; i++)
    {
        if (Wthrphnmn[i][0] == '\0')
        {
           break;
        }

        int whtrindex = 0;

        const char* Segment = Wthrphnmn[i];

        const char* intensitydscrp = "";

        for (int j = 0; j < 2; j++)
        {
            if (Segment[whtrindex] == intensity[j].code[0])
            {
                intensitydscrp = intensity[j].description;
                ++whtrindex;
                break;
            }
        }

        for (const char* p = intensitydscrp; *p != '\0'; ++p) 
        {
            translation[translateindex++] = *p;
        }

        while (Segment[whtrindex] != '\0')
        {
            char code[3] = {Segment[whtrindex], Segment[whtrindex + 1], '\0'};
            bool exists = false;

            for (int j = 0; j < 29; j++)
            {
                if (code[0] == weather[j].code[0] && code[1] == weather[j].code[1]) {
                    for (const char* p = weather[j].description; *p != '\0'; ++p) {
                        translation[translateindex++] = *p;
                    }
                    translation[translateindex++] = ' ';
                    exists = true;
                    break;
                }
            }
            if (!exists)
            {
                const char unknown[] = "nieznane_zjawisko(";
                for (const char* p = unknown; *p != '\0'; ++p) 
                {
                    translation[translateindex++] = *p;
                }
                translation[translateindex++] = code[0];
                translation[translateindex++] = code[1];
                translation[translateindex++] = ')';
                translation[translateindex++] = ' ';
            }
            whtrindex += 2;
        } 
    }
    translation[translateindex] = '\0';
    return translation;
}
void DisplayData(const METARdata& data)
{
    cout << "Lotnisko(ICAO):            " << TranslateICAO(data.ICAO) << endl;
    cout << "Dzien miesiaca:            " << data.Day << endl;
    cout << "Czas UTC:                  " << data.UTCtime[0] << data.UTCtime[1] << ":" << data.UTCtime[2] << data.UTCtime[3] << endl;
    cout << "Wiatr w kierunku           " << data.WindDir << "\u00B0";
    if (data.WindDirCh[0] != '\0')
    {
        cout << " zmiana kierunku wiatru miedzy: " << data.WindDirCh[0] << data.WindDirCh[1] << data.WindDirCh[2] << "\u00B0, a " << data.WindDirCh[4] << data.WindDirCh[5] << data.WindDirCh[6] << "\u00B0" << endl;
    }
    else
    {
        cout << endl;
    }
    cout << "predkosc wiatru:           " << data.WindSpd  << " kt";

    if (data.WindGst[0] != '\0')
    {
        cout << ", w porywach do: " << data.WindGst << " kt" << endl;
    }
    else
    {
        cout << endl;
    }


    
    cout << "Widzialnosc:               ";
    if (data.Visblty[0] == 'C')
    {
        cout << "chmury i widoczność są w porządku" << endl;
    }
    else
    {
        cout << data.Visblty << " m" << endl;
    }

    if (data.Wthrphnm[0][0] != '\0')
    {
        cout << "Zjawiska atmosferyczne:    " << TranslateWeather(data.Wthrphnm) << endl;
    }
    

    cout << "Zachmurzenie:" << endl;
    for (int i = 0; i < data.Cloudcount; i++) {
        cout << "   Typ:                    " << data.Clouds[i] 
             << ", Podstawa: " << data.Cldhght[i] << " ft" << endl;
    }
    cout << "Temperatura:               " << data.tmp << "\u00B0C"<< endl; 
    cout << "temperatura pktu rosy:     " << data.dewtmp << "\u00B0C" << endl;
    cout << "Cisnienie atmosferyczne:   " << data.atmprs << " hPa" << endl << endl;
}

void SaveToFile(ofstream& outfile, METARdata& data)
{
    outfile << "Lotnisko(ICAO):            " << TranslateICAO(data.ICAO) << endl;
    outfile << "Dzień miesiąca:            " << data.Day << endl;
    outfile << "Czas UTC:                  " << data.UTCtime[0] << data.UTCtime[1] << ":" << data.UTCtime[2] << data.UTCtime[3] << endl;
    outfile << "Wiatr w kierunku           " << data.WindDir << "\u00B0";
    if (data.WindDirCh[0] != '\0')
    {
        outfile << " zmiana kierunku wiatru miedzy: " << data.WindDirCh[0] << data.WindDirCh[1] << data.WindDirCh[2] << "\u00B0, a " << data.WindDirCh[4] << data.WindDirCh[5] << data.WindDirCh[6] << "\u00B0" << endl;
    }
    else
    {
        outfile << endl;
    }
    outfile << "predkosc wiatru:           " << data.WindSpd << " kt";

    if (data.WindGst[0] != '\0')
    {
        outfile << ", w porywach do: " << data.WindGst << " kt" << endl;
    }
    else
    {
        outfile << endl;
    }

    outfile << "Widzialnosc:               ";
    if (data.Visblty[0] == 'C')
    {
        outfile << "chmury i widoczność są w porządku" << endl;
    }
    else
    {
        outfile << data.Visblty << " m" << endl;
    }

    if (data.Wthrphnm[0][0] != '\0')
    {
        outfile << "Zjawiska atmosferyczne:    " << TranslateWeather(data.Wthrphnm) << endl;
    }

    outfile << "Zachmurzenie:" << endl;
    for (int i = 0; i < data.Cloudcount; i++) {
        outfile << "   Typ:                    " << data.Clouds[i]
            << ", Podstawa: " << data.Cldhght[i] << " ft" << endl;
    }
    outfile << "Temperatura:               " << data.tmp << "\u00B0C" << endl;
    outfile << "temperatura pktu rosy:     " << data.dewtmp << "\u00B0C" << endl;
    outfile << "Cisnienie atmosferyczne:   " << data.atmprs << " hPa" << endl << endl;
}

void LastThreeMeasurments(METARdata lastmsrmnts[3], int counter)
{
    int quantity = 3;
    if (counter < 3) {
        quantity = counter;
    }

    for (int i = quantity - 1; i >= 0; i--) 
    {
        int index = (counter - 1 - i) % 3;
        DisplayData(lastmsrmnts[index]);
    }

}

void ParseMETAR(const char* line, METARdata& data, ofstream& outfile)
{
    int index = 0;

    ICAOCode(line, index, data);
    DayNumber(line, index, data);
    UTC(line, index, data);
    WindDirecton(line, index, data);
    WindSpeed(line, index, data);
    if (line[index] == 'G')
    {
        WindGust(line, index, data);
    }
    index+=3;

    if (line[index+3] == 'V')
    {
        WindDirectionChange(line, index, data);
    }

    Visiblity(line, index, data);
    WeatherPhenomena(line, index, data);
    CloudsInfo(line, index, data);
    Temperature(line, index, data);
    dewTemperature(line, index, data);
    AtmosphericPressure(line, index, data);

    SaveToFile(outfile, data);
}

void FileConversion(const char inputfile[105], const char outputfile[105]) 
{

    METARdata lastmsrmnts[3];
    int counter = 0;

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
        METARdata Metar = {};
        ParseMETAR(line,Metar ,outfile);
        lastmsrmnts[counter % 3] = Metar;
        counter++;
    }

    LastThreeMeasurments(lastmsrmnts, counter);

    infile.close();
    outfile.close();

    cout << "Program zakonczyl dzialanie" << endl;

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