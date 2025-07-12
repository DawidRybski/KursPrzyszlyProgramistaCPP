#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <windows.h>

using namespace std;

struct Person
{
    int id;
    int userId;
    string name;
    string surname;
    string phoneNumber;
    string email;
    string address;
};

struct User
{
    int id;
    string username, password;
};

void registration(vector <User> &users);
int login(vector <User> &users, int usersNumber);
void changePassword(vector <User> &users, int usersNumber, int loggedUserId);
void readUsersFromFile(vector <User> &users);
void overrideUsersFile (vector <User> &users);
bool comparePeopleId(const Person &a, const Person &b);
string readLine();
char readChar();
int readInt();
void addPerson(vector <Person> &people, int loggedUserId);
void readPeopleFromFile(vector <Person> &people, int loggedUserId);
void overrideFileByErase (const Person &erasedPerson);
void overrideFileByModify (const Person &modifiedPerson);
void showPeopleByName (vector <Person> &people);
void showPeopleBySurname (vector <Person> &people);
void showPeopleList (vector <Person> &people);
void erasePerson (vector <Person> &people);
void editPerson (vector <Person> &people);

int main()
{
    vector <Person> people;
    vector <User> users;
    int loggedUserId = 0;
    int usersNumber = 0;
    int peopleNumber = 0;
    bool run = true;
    char choice;
    readUsersFromFile(users);

    system("cls");
    while (run)
    {
        if (loggedUserId == 0)
        {
            system("cls");
            cout << ">>> MENU GLOWNE <<<" << endl << endl;
            cout << "1. Rejestracja" << endl;
            cout << "2. Logowanie" << endl;
            cout << "9. Zakoncz program" << endl;
            cout << "Twoj wybor: ";

            choice = readChar();

            if (choice == '1')
            {
                registration(users);
            }
            else if (choice == '2')
            {
                loggedUserId = login(users, usersNumber);
            }
            else if (choice == '9')
            {
                exit(0);
            }
        }
        else
        {
            readPeopleFromFile(people, loggedUserId);
            system("cls");
            cout << ">>> MENU UZYTKOWNIKA <<<" << endl;
            cout << "----------------------------" << endl;
            cout << "1. Dodaj adresata" << endl;
            cout << "2. Wyszukaj po imieniu" << endl;
            cout << "3. Wyszukaj po nazwisku" << endl;
            cout << "4. Wyswietl wszystkich adresatow" << endl;
            cout << "5. Usun adresata" << endl;
            cout << "6. Edytuj adresata" << endl;
            cout << "----------------------------" << endl;
            cout << "7. Zmiana hasla" << endl;
            cout << "8. Wyloguj sie" << endl;
            cout << "----------------------------" << endl;
            cout << "Twoj wybor: ";
            choice = readChar();

            switch(choice)
            {
                case '1':
                    system("cls");
                    addPerson(people, loggedUserId);
                    break;
                case '2':
                    system("cls");
                    showPeopleByName(people);
                    break;
                case '3':
                    system("cls");
                    showPeopleBySurname(people);
                    break;
                case '4':
                    system("cls");
                    showPeopleList(people);
                    break;
                case '5':
                    system("cls");
                    erasePerson(people);
                    break;
                case '6':
                    system("cls");
                    editPerson(people);
                    break;
                case '7':
                    system("cls");
                    changePassword(users, usersNumber, loggedUserId);
                    break;
                case '8':
                    system("cls");
                    loggedUserId = 0;
                    break;
            }
        }
    }
}

void registration(vector <User> &users)
{
    string username, password;

    cout << "Podaj nazwe uzytkownika: ";
    username = readLine();

    for (const auto& u : users)
    {
        if (u.username == username)
        {
            cout << "Taki uzytkownik juz istnieje. Rejestracja anulowana." << endl;
            Sleep(2000);
            return;
        }
    }

    cout << "Podaj password: ";
    password = readLine();

    User newUser;
    newUser.id = users.empty() ? 1 : users.back().id + 1;
    newUser.username = username;
    newUser.password = password;
    users.push_back(newUser);

    overrideUsersFile(users);

    cout << "Konto zalozone" << endl;
    Sleep(1000);
}

int login(vector <User> &users, int usersNumber)
{
    string username, password;
    cout << "Podaj login: ";
    username = readLine();

    for (const auto& u : users)
    {
        if (u.username == username)
        {
            for (int attempts = 0; attempts < 3; attempts++)
            {
                cout << "Podaj password. Pozostalo prob: " << 3 - attempts << ": ";
                password = readLine();

                if (u.password == password)
                {
                    cout << "Zalogowales sie." << endl;
                    Sleep(1000);
                    return u.id;
                }
            }

            cout << "3 nieudane attempts. Zaczekaj 3 sekundy." << endl;
            Sleep(3000);
            return 0;
        }
    }

    cout << "Nie ma uzytkownika z takim loginem." << endl;
    Sleep(2000);
    return 0;
}

void changePassword(vector <User> &users, int usersNumber, int loggedUserId)
{
    string password;
    cout << "Podaj nowe password: ";
    password = readLine();

    for (auto& u : users)
    {
        if (u.id == loggedUserId)
        {
            u.password = password;
            cout << "Haslo zostalo zmienione." << endl;
            overrideUsersFile(users);
            Sleep(2000);
            return;
        }
    }
}

void readUsersFromFile(vector <User> &users)
{
    users.clear();
    string line = "";

    fstream file;
    file.open("Uzytkownicy.txt", ios::in);

    User user;
    string data = "";

    while(getline(file, line))
    {
        int nextData = 0;
        stringstream ss(line);

        while (getline(ss, data, '|'))
        {
            switch (nextData)
            {
                case 0: user.id = stoi(data); break;
                case 1: user.username = data; break;
                case 2: user.password = data;
            }
            nextData++;
            data = "";
        }
        users.push_back(user);
    }
    file.close();
}

void overrideUsersFile (vector <User> &users)
{
    fstream file;
    file.open("Uzytkownicy.txt", ios::out);

    if (file.good())
    {
        for (User &user : users)
        {
            file << user.id << "|";
            file << user.username << "|";
            file << user.password << endl;
        }
        file.close();
    }
    else
    {
        cout << "Nie udalo sie otworzyc pliku do zapisu." << endl;
        system("pause");
    }
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

void addPerson(vector <Person> &people, int loggedUserId)
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
    else
    {
        cout << "Nie udalo sie otworzyc pliku do odczytu." << endl;
    }
    file.close();

    Person newPerson;
    newPerson.id = lastId + 1;
    newPerson.userId = loggedUserId;
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
        << newPerson.userId << "|"
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

void readPeopleFromFile(vector <Person> &people, int loggedUserId)
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
                case 1: person.userId = stoi(data); break;
                case 2: person.name = data; break;
                case 3: person.surname = data; break;
                case 4: person.phoneNumber = data; break;
                case 5: person.email = data; break;
                case 6: person.address = data;
            }
            nextData++;
            data = "";
        }

        if (person.userId == loggedUserId)
        {
            people.push_back(person);
        }
    }
    file.close();
}

void overrideFileByErase (const Person &erasedPerson)
{
    fstream file;
    file.open("ksiazka_adresowa.txt", ios::in);

    fstream tempFile;
    tempFile.open("ksiazka_adresowa_tymczasowa.txt", ios::out);

    string line = "";

    if (!file.good() || !tempFile.good())
    {
        cout << "Nie udalo sie otworzyc plikow." << endl;
        system("pause");
        return;
    }

    while(getline(file, line))
    {
        stringstream ss(line);
        string idStr;

        getline(ss, idStr, '|');
        int id = stoi(idStr);

        if (erasedPerson.id == stoi(idStr))
        {
            continue;
        }
        else
        {
            tempFile << line << endl;
        }
    }
    file.close();
    tempFile.close();

    remove("ksiazka_adresowa.txt");
    rename("ksiazka_adresowa_tymczasowa.txt", "ksiazka_adresowa.txt");
}

void overrideFileByModify (const Person &modifiedPerson)
{
    fstream file;
    file.open("ksiazka_adresowa.txt", ios::in);

    fstream tempFile;
    tempFile.open("ksiazka_adresowa_tymczasowa.txt", ios::out);

    string line = "";

    if (!file.good() || !tempFile.good())
    {
        cout << "Nie udalo sie otworzyc plikow." << endl;
        system("pause");
        return;
    }

    while(getline(file, line))
    {
        stringstream ss(line);
        string idStr;

        getline(ss, idStr, '|');
        int id = stoi(idStr);

        if (modifiedPerson.id == stoi(idStr))
        {
            tempFile << modifiedPerson.id << "|";
            tempFile << modifiedPerson.userId << "|";
            tempFile << modifiedPerson.name << "|";
            tempFile << modifiedPerson.surname << "|";
            tempFile << modifiedPerson.phoneNumber << "|";
            tempFile << modifiedPerson.email << "|";
            tempFile << modifiedPerson.address << endl;
        }
        else
        {
            tempFile << line << endl;
        }
    }
    file.close();
    tempFile.close();

    remove("ksiazka_adresowa.txt");
    rename("ksiazka_adresowa_tymczasowa.txt", "ksiazka_adresowa.txt");
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

    for (Person person: people)
    {
        if (person.id == personId)
        {
            foundId = true;
        }
    }

    if (!foundId)
    {
        cout << "Nie znaleziono adresata o podanym ID." << endl;
        system("pause");
        return;
    }

    cout << "Czy na pewno chcesz usunac adresata? (Wprowadz 't', aby potwierdzic lub inny znak, aby anulowac operacje):";
    char confirmation = readChar();

    if (confirmation == 't')
    {
       for (vector<Person>::iterator itr = people.begin(); itr != people.end(); itr++)
        {
            if (itr->id == personId)
            {
                overrideFileByErase(*itr);
                people.erase(itr);
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
        Person modifiedPerson;
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
                        modifiedPerson = person;
                        break;
                    }
                }
                overrideFileByModify(modifiedPerson);
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
                        modifiedPerson = person;
                        break;
                    }
                }
                overrideFileByModify(modifiedPerson);
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
                        modifiedPerson = person;
                        break;
                    }
                }
                overrideFileByModify(modifiedPerson);
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
                        modifiedPerson = person;
                        break;
                    }
                }
                overrideFileByModify(modifiedPerson);
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
                        modifiedPerson = person;
                        break;
                    }
                }
                overrideFileByModify(modifiedPerson);
                cout << "Adres zamieszkania adresata zostalo zmienione." << endl;
                break;
            }
            case '6':
                runEditPerson = false;
                break;
        }
    }
}
