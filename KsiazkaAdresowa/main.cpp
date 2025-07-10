#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Person
{
    int id;
    string name;
    string surname;
    string phoneNumber;
    string email;
    string address;
};

bool comparePeopleId(const Person &a, const Person &b);
string readLine();
char readChar();
int readInt();
void addPerson(vector <Person> &people);
void readPeopleFromFile(vector <Person> &people);
void overrideFile (vector <Person> &people);
void showPeopleByName (vector <Person> &people);
void showPeopleBySurname (vector <Person> &people);
void showPeopleList (vector <Person> &people);
void erasePerson (vector <Person> &people);
void editPerson (vector <Person> &people);

int main()
{
    vector <Person> people;
    int peopleNumber = 0;
    bool run = true;
    char choice;

    readPeopleFromFile(people);

    system("cls");
    while (run)
    {
        system("cls");
        cout << ">>> KSIAZKA ADRESOWA <<<" << endl << endl;
        cout << "1. Dodaj adresata" << endl;
        cout << "2. Wyszukaj po imieniu" << endl;
        cout << "3. Wyszukaj po nazwisku" << endl;
        cout << "4. Wyswietl wszystkich adresatow" << endl;
        cout << "5. Usun adresata" << endl;
        cout << "6. Edytuj adresata" << endl;
        cout << "9. Zakoncz program" << endl << endl;
        cout << "Twoj wybor: ";


        choice = readChar();
        switch(choice)
        {
            case '1':
                addPerson(people);
                break;
            case '2':
                showPeopleByName(people);
                break;
            case '3':
                showPeopleBySurname(people);
                break;
            case '4':
                showPeopleList(people);
                break;
            case '5':
                erasePerson(people);
                break;
            case '6':
                editPerson(people);
                break;
            case '9':
                run = false;
                break;
        }

    }
    return 0;
}

bool comparePeopleId(const Person &a, const Person &b)
{
	return a.id < b.id;
}

string readLine()
{
    string entry = "";
    cin.sync();
    getline(cin, entry);
    return entry;
}


char readChar()
{
    string entry = "";
    char character = {0};

    while (true)
    {
        getline(cin, entry);

        if (entry.length() == 1)
        {
            character = entry[0];
            break;
        }
        cout << "Nie podales pojedynczego znaku. Wpisz ponownie!" << endl;
    }

    return character;
}

int readInt()
{
    string entry = "";
    int number = 0;

    while (true)
    {
        getline(cin, entry);

        stringstream myStream(entry);
        if (myStream >> number)
            break;
        cout << "To nie jest liczba. Wpisz ponownie." << endl;
    }
    return number;
}

void addPerson(vector <Person> &people)
{
    string name, surname, phoneNumber, email, address;
    cout << "Podaj imie:";
    name = readLine();
    cout << "Podaj nazwisko:";
    surname = readLine();
    cout << "Podaj numer telefonu:";
    phoneNumber = readLine();
    cout << "Podaj adres email:";
    email = readLine();
    cout << "Podaj adres zamieszkania:";
    address = readLine();

    fstream file;
    file.open("ksiazka_adresowa.txt", ios::in);

    int lastId = 0;
    if (file.good())
    {
        string line;
        while(getline(file, line))
        {
            stringstream ss(line);
            string idStr;
            getline(ss, idStr, '|');
            int id = stoi(idStr);
            if (id > lastId)
                lastId = id;
        }
    }
    file.close();

    Person newPerson;

    newPerson.id = lastId + 1;
    newPerson.name = name;
    newPerson.surname = surname;
    newPerson.phoneNumber = phoneNumber;
    newPerson.email = email;
    newPerson.address = address;
    people.push_back(newPerson);
    sort(people.begin(), people.end(), comparePeopleId);

    fstream fileOverride;
    fileOverride.open("ksiazka_adresowa.txt", ios::out | ios::app);

    if (fileOverride.good())
    {
        fileOverride
        << newPerson.id << "|"
        << newPerson.name << "|"
        << newPerson.surname << "|"
        << newPerson.phoneNumber << "|"
        << newPerson.email << "|"
        << newPerson.address << endl;
        fileOverride.close();
    }
    else
    {
        cout << "Nie udalo sie otworzyc pliku do zapisu." << endl;
    }
    fileOverride.close();
    cout << endl << "Osoba zostala dodana" << endl; system("pause");
}


void readPeopleFromFile(vector <Person> &people)
{
    people.clear();
    string line = "";

    fstream file;
    file.open("ksiazka_adresowa.txt", ios::in);

    Person person;
    string data = "";

    while(getline(file, line))
    {
        int nextData = 0;
        stringstream ss(line);

        while (getline(ss, data, '|'))
        {
            switch (nextData)
            {
                case 0: person.id = stoi(data); break;
                case 1: person.name = data; break;
                case 2: person.surname = data; break;
                case 3: person.phoneNumber = data; break;
                case 4: person.email = data; break;
                case 5: person.address = data;
            }
            nextData++;
            data = "";
        }
        people.push_back(person);
    }
    file.close();
}

void overrideFile (vector <Person> &people)
{
    fstream file;
    file.open("ksiazka_adresowa.txt", ios::out);

    if (file.good())
    {
        for (Person &person : people)
        {
            file << person.id << "|";
            file << person.name << "|";
            file << person.surname << "|";
            file << person.phoneNumber << "|";
            file << person.email << "|";
            file << person.address << endl;
        }
        file.close();
    }
    else
    {
        cout << "Nie udalo sie otworzyc pliku do zapisu." << endl;
        system("pause");
    }
}

void showPeopleByName (vector <Person> &people)
{
    int nameCount = 0;
    cout << "Podaj imie adresata: ";
    string searchedName = readLine();

    for (Person person: people)
    {
        if (person.name == searchedName)
        {
            cout << person.id << endl;
            cout << person.name << endl;
            cout << person.surname << endl;
            cout << person.phoneNumber << endl;
            cout << person.email << endl;
            cout << person.address << endl << endl;
            nameCount++;
        }
    }
    cout << endl;

    if (nameCount == 0)
        cout << "Brak osob z podanym imieniem w ksiazce adresowej." << endl << endl;

    system("pause");
}

void showPeopleBySurname (vector <Person> &people)
{
    int surnameCount = 0;
    cout << "Podaj nazwisko adresata: ";
    string searchedSurname = readLine();

    for (Person person: people)
    {
        if (person.surname == searchedSurname)
        {
            cout << person.id << endl;
            cout << person.name << endl;
            cout << person.surname << endl;
            cout << person.phoneNumber << endl;
            cout << person.email << endl;
            cout << person.address << endl << endl;
            surnameCount++;
        }
    }
    cout << endl;

    if (surnameCount == 0)
        cout << "Brak osob z podanym nazwiskiem w ksiazce adresowej." << endl << endl;

    system("pause");
}

void showPeopleList (vector <Person> &people)
{
    int peopleCount = 0;

    for (Person person: people)
    {
            cout << "Id: " << person.id << endl;
            cout << "Imie: " << person.name << endl;
            cout << "Nazwisko: " << person.surname << endl;
            cout << "Numer telefonu: " << person.phoneNumber << endl;
            cout << "email: " << person.email << endl;
            cout << "Adres: " << person.address << endl << endl;
            peopleCount++;
    }

    if (peopleCount == 0)
        cout << "Lista adresowa jest pusta." << endl << endl;

    system("pause");
}

void erasePerson (vector <Person> &people)
{
    cout << "Podaj id adresata:";
    int personId = readInt();
    bool foundId = false;

    cout << "Czy na pewno chcesz usunac adresata? (Wprowadz 't', aby potwierdzic lub inny znak, aby anulowac operacje):";
    char confirmation = readChar();

    if (confirmation == 't')
    {
       for (vector<Person>::iterator itr = people.begin(); itr != people.end(); itr++)
        {
            if (itr->id == personId)
            {
                people.erase(itr);
                overrideFile(people);
                cout << "Adresat zostal usuniety." << endl;
                foundId = true;
                break;
            }
        }
    }
    else
    {
        cout << "Operacja usuniecia adresata zostala anulowana" << endl;
    }

    system("pause");
}

void editPerson (vector <Person> &people)
{
    cout << "Wprowadz id adresata, ktorego chcesz edytowac: ";
    int personId = readInt();
    bool runEditPerson = true;

    while (runEditPerson)
    {
        cout << endl << "Wybierz, ktore dane chcesz edytowac:" << endl;
        cout << "1 - imie" << endl;
        cout << "2 - nazwisko" << endl;
        cout << "3 - numer telefonu" << endl;
        cout << "4 - email" << endl;
        cout << "5 - adres" << endl;
        cout << "6 - powrot do menu" << endl << endl;
        cout << "Twoj wybor: ";


        char choice = readChar();
        switch(choice)
        {
            case '1':
            {
                system("cls");
                cout << "Wprowadz nowe imie:";
                for (Person &person : people)
                {
                    if (person.id == personId)
                    {
                        person.name = readLine();
                        break;
                    }
                }
                overrideFile(people);
                cout << "Imie adresata zostalo zmienione." << endl;
                break;
            }

            case '2':
            {
                system("cls");
                cout << "Wprowadz nowe nazwisko:";
                for (Person &person : people)
                {
                    if (person.id == personId)
                    {
                        person.surname = readLine();
                        break;
                    }
                }
                overrideFile(people);
                cout << "Nazwisko adresata zostalo zmienione." << endl;
                break;
            }
            case '3':
            {
                system("cls");
                cout << "Wprowadz nowy numer telefonu:";
                for (Person &person : people)
                {
                    if (person.id == personId)
                    {
                        person.phoneNumber = readLine();
                        break;
                    }
                }
                overrideFile(people);
                cout << "Numer telefonu adresata zostalo zmienione." << endl;
                break;
            }
            case '4':
            {
                system("cls");
                cout << "Wprowadz nowy email:";
                for (Person &person : people)
                {
                    if (person.id == personId)
                    {
                        person.email = readLine();
                        break;
                    }
                }
                overrideFile(people);
                cout << "Email adresata zostalo zmienione." << endl;
                break;
            }
            case '5':
            {
                system("cls");
                cout << "Wprowadz nowy adres:";
                for (Person &person : people)
                {
                    if (person.id == personId)
                    {
                        person.address = readLine();
                        break;
                    }
                }
                overrideFile(people);
                cout << "Adres zamieszkania adresata zostalo zmienione." << endl;
                break;
            }
            case '6':
                runEditPerson = false;
                break;
        }
    }
}
