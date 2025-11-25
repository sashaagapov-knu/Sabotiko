#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <sstream> // Додано для зручного парсингу даних

using namespace std;

// --- КОНСТАНТИ ---
const int MAX_BOOKS = 100;
const int MAX_REVIEWS = 100;
const string LIBRARY_FILE = "library_data.txt"; // Назва файлу для книг
const string REVIEWS_FILE = "reviews_data.txt"; // Назва файлу для відгуків

// --- СТРУКТУРИ ---
struct Book
{
    int id;
    string title;
    string author;
};

struct Review
{
    int bookId;
    string userName;
    string text;
    int rating;
};

// -------------ПРОТОТИПИ ФУНКЦІЙ----------------
void inputBooks(struct Book library[], int& count);
void displayBooks(struct Book library[], int count);
void editBook(struct Book library[], int count);
void searchBooks(struct Book library[], int count);
void addReview(struct Review reviews[], int& reviewCount);
void displayReviews(struct Review reviews[], int reviewCount);

// --- ФУНКЦІЇ ЗБЕРЕЖЕННЯ/ЗАВАНТАЖЕННЯ ---
void saveData(const Book library[], int count, const Review reviews[], int reviewCount);
void loadData(Book library[], int& count, Review reviews[], int& reviewCount);

// --- ДОПОМІЖНА ФУНКЦІЯ ОЧИЩЕННЯ ВВЕДЕННЯ ---
void clearInput() {
    if (cin.fail()) {
        cin.clear();
    }
    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
}

// =================================================================
//  ФУНКЦІЇ УПРАВЛІННЯ ДАНИМИ
// =================================================================

/**
 * Зберігає дані про книги та відгуки у файли.
 * Формат: ID|Назва|Автор (для книг) та BookID|Користувач|Текст|Оцінка (для відгуків).
 */
void saveData(const Book library[], int count, const Review reviews[], int reviewCount)
{
    // 1. Збереження книг
    ofstream libFile(LIBRARY_FILE);
    if (libFile.is_open())
    {
        for (int i = 0; i < count; i++)
        {
            // Використовуємо | як роздільник
            libFile << library[i].id << "|" << library[i].title << "|" << library[i].author << "\n";
        }
        libFile.close();
    } else {
        cout << "\n Помилка: Не вдалося відкрити файл " << LIBRARY_FILE << " для запису.\n";
    }

    // 2. Збереження відгуків
    ofstream revFile(REVIEWS_FILE);
    if (revFile.is_open())
    {
        for (int i = 0; i < reviewCount; i++)
        {
            // Використовуємо | як роздільник
            revFile << reviews[i].bookId << "|" << reviews[i].userName << "|" << reviews[i].text << "|" << reviews[i].rating << "\n";
        }
        revFile.close();
    } else {
        cout << "\n Помилка: Не вдалося відкрити файл " << REVIEWS_FILE << " для запису.\n";
    }
}

/**
 * Завантажує дані про книги та відгуки з файлів.
 */
void loadData(Book library[], int& count, Review reviews[], int& reviewCount)
{
    count = 0; // Скидаємо лічильники
    reviewCount = 0;

    // 1. Завантаження книг
    ifstream libFile(LIBRARY_FILE);
    if (libFile.is_open())
    {
        string line;
        while (getline(libFile, line) && count < MAX_BOOKS)
        {
            stringstream ss(line);
            string segment;
            Book tempBook;
            int segmentIndex = 0;

            // Парсинг рядка з роздільником |
            while (getline(ss, segment, '|'))
            {
                if (segmentIndex == 0) {
                    try {
                        tempBook.id = stoi(segment);
                    } catch (...) {
                        // Ігноруємо рядок з помилкою
                        break;
                    }
                } else if (segmentIndex == 1) {
                    tempBook.title = segment;
                } else if (segmentIndex == 2) {
                    tempBook.author = segment;
                }
                segmentIndex++;
            }

            if (segmentIndex == 3) {
                library[count++] = tempBook;
            }
        }
        libFile.close();
        cout << " Завантажено " << count << " книг.\n";
    } else {
        cout << " Файл " << LIBRARY_FILE << " не знайдено. Починаємо з порожньої бібліотеки.\n";
    }

    // 2. Завантаження відгуків
    ifstream revFile(REVIEWS_FILE);
    if (revFile.is_open())
    {
        string line;
        while (getline(revFile, line) && reviewCount < MAX_REVIEWS)
        {
            stringstream ss(line);
            string segment;
            Review tempReview;
            int segmentIndex = 0;

            // Парсинг рядка з роздільником |
            while (getline(ss, segment, '|'))
            {
                if (segmentIndex == 0) {
                    try {
                        tempReview.bookId = stoi(segment);
                    } catch (...) {
                        break;
                    }
                } else if (segmentIndex == 1) {
                    tempReview.userName = segment;
                } else if (segmentIndex == 2) {
                    tempReview.text = segment;
                } else if (segmentIndex == 3) {
                    try {
                        tempReview.rating = stoi(segment);
                    } catch (...) {
                        break;
                    }
                }
                segmentIndex++;
            }

            if (segmentIndex == 4) {
                reviews[reviewCount++] = tempReview;
            }
        }
        revFile.close();
        cout << " Завантажено " << reviewCount << " відгуків.\n";
    } else {
        cout << " Файл " << REVIEWS_FILE << " не знайдено. Починаємо без відгуків.\n";
    }
}


// =================================================================
// 📖 ФУНКЦІЇ БІБЛІОТЕКИ (оновлені для використання clearInput)
// =================================================================

void inputBooks(Book library[], int& count)
{
    int n;
    cout << "Скільки книг бажаєте додати? ";
    cin >> n;

    clearInput(); // Використовуємо допоміжну функцію

    for (int i = 0; i < n; i++)
    {
        if (count >= MAX_BOOKS)
        {
            cout << "Досягнуто максимальну кількість книг у бібліотеці.\n";
            return;
        }

        cout << "\nКнига #" << count + 1 << endl;

        cout << "ID книги: ";
        // Уникаємо `cin.ignore` тут, оскільки `clearInput` вже його викликала
        if (!(cin >> library[count].id)) {
            cout << "Помилка: Некоректний ID. Пропускаємо книгу.\n";
            clearInput();
            continue;
        }
        clearInput(); // Очищуємо буфер після введення числа

        cout << "Назва книги: ";
        getline(cin, library[count].title);

        cout << "Автор: ";
        getline(cin, library[count].author);

        cout << "Книгу успішно додано!\n";
        count++;
    }
}

void displayBooks(Book library[], int count)
{
    if (count == 0)
    {
        cout << "\nБібліотека поки порожня.\n";
        return;
    }

    cout << "\nСписок доданих книг:\n";
    cout << "-----------------------------------------\n";
    for (int i = 0; i < count; i++)
    {
        cout << "ID: " << library[i].id
            << " | Назва: " << library[i].title
            << " | Автор: " << library[i].author << endl;
    }
    cout << "-----------------------------------------\n";
}

void editBook(Book library[], int count)
{
    if (count == 0)
    {
        cout << "\nБібліотека порожня. Немає чого редагувати.\n";
        return;
    }

    int targetId;
    cout << "\nВведіть ID книги, яку бажаєте редагувати: ";
    if (!(cin >> targetId)) {
        cout << "Помилка: Некоректний ID.\n";
        clearInput();
        return;
    }

    bool found = false;
    for (int i = 0; i < count; i++)
    {
        if (library[i].id == targetId)
        {
            found = true;
            cout << "Знайдено книгу: " << library[i].title << " (" << library[i].author << ")\n";

            clearInput(); // Очищуємо буфер після введення числа

            cout << "Введіть нову назву (або Enter, щоб залишити поточну: '" << library[i].title << "'): ";
            string newTitle;
            getline(cin, newTitle);

            cout << "Введіть нового автора (або Enter, щоб залишити поточного: '" << library[i].author << "'): ";
            string newAuthor;
            getline(cin, newAuthor);

            if (!newTitle.empty())
            {
                library[i].title = newTitle;
            }
            if (!newAuthor.empty())
            {
                library[i].author = newAuthor;
            }

            cout << "Дані книги (ID: " << library[i].id << ") успішно оновлено!\n";
            break;
        }
    }

    if (!found)
    {
        cout << "Книгу з ID " << targetId << " не знайдено.\n";
    }
}


void searchBooks(Book library[], int count)
{
    if (count == 0)
    {
        cout << "\nБібліотека порожня. Немає чого шукати.\n";
        return;
    }

    int choice;
    cout << "\n--- Меню пошуку --- \n";
    cout << "1. Пошук за ID\n";
    cout << "2. Пошук за назвою\n";
    cout << "3. Пошук за автором\n";
    cout << "0. Повернутися в головне меню\n";
    cout << "Ваш вибір: ";
    if (!(cin >> choice)) {
        cout << "Помилка: Некоректний вибір.\n";
        clearInput();
        return;
    }
    clearInput(); // Очищуємо буфер після введення числа

    bool found = false;

    switch (choice)
    {
    case 1:
    {
        int targetId;
        cout << "Введіть ID для пошуку: ";
        if (!(cin >> targetId)) {
            cout << "Помилка: Некоректний ID.\n";
            clearInput();
            return;
        }
        clearInput();

        cout << "\nРезультати пошуку за ID:\n";
        for (int i = 0; i < count; i++)
        {
            if (library[i].id == targetId)
            {
                cout << "ID: " << library[i].id << " | Назва: " << library[i].title << " | Автор: " << library[i].author << endl;
                found = true;
                break;
            }
        }
        break;
    }
    case 2:
    {
        string searchTerm;
        cout << "Введіть назву для пошуку (або її частину): ";
        getline(cin, searchTerm);

        cout << "\nРезультати пошуку за назвою '" << searchTerm << "':\n";
        for (int i = 0; i < count; i++)
        {
            // Використовуємо .find для пошуку частини рядка
            if (library[i].title.find(searchTerm) != string::npos)
            {
                cout << "ID: " << library[i].id << " | Назва: " << library[i].title << " | Автор: " << library[i].author << endl;
                found = true;
            }
        }
        break;
    }
    case 3:
    {
        string searchTerm;
        cout << "Введіть автора для пошуку (або його частину): ";
        getline(cin, searchTerm);

        cout << "\nРезультати пошуку за автором '" << searchTerm << "':\n";
        for (int i = 0; i < count; i++)
        {
            if (library[i].author.find(searchTerm) != string::npos)
            {
                cout << "ID: " << library[i].id << " | Назва: " << library[i].title << " | Автор: " << library[i].author << endl;
                found = true;
            }
        }
        break;
    }
    case 0:
        return;
    default:
        cout << "Невірний вибір.\n";
        return;
    }

    if (!found)
    {
        cout << "Нічого не знайдено.\n";
    }
}

void addReview(Review reviews[], int& reviewCount)
{
    if (reviewCount >= MAX_REVIEWS)
    {
        cout << endl
            << "Досягнуто максимальну кількість відгуків." << endl;
        return;
    }

    cout << endl
        << "--- Додавання нового відгуку --- " << endl;

    cout << "Введіть ID книги, до якої додаєте відгук: ";
    if (!(cin >> reviews[reviewCount].bookId)) {
        cout << "Помилка: Некоректний ID.\n";
        clearInput();
        return;
    }

    clearInput(); // Очищуємо буфер після введення числа

    cout << "Ваше ім'я: ";
    getline(cin, reviews[reviewCount].userName);
    cout << "Текст відгуку: ";
    getline(cin, reviews[reviewCount].text);

    int rating = 0;

    while (true)
    {
        cout << "Оцінка (1–5): ";
        if (cin >> rating && rating >= 1 && rating <= 5)
        {
            reviews[reviewCount].rating = rating;
            break;
        }
        else
        {
            cout << "Помилка: введіть число від 1 до 5." << endl;
            clearInput();
        }
    }
    clearInput();

    cout << "Відгук успішно додано!" << endl;
    reviewCount++;
}

void displayReviews(Review reviews[], int reviewCount)
{
    if (reviewCount == 0)
    {
        cout << "\nВідгуків поки немає.\n";
        return;
    }

    cout << "\nСписок доданих відгуків:\n";
    cout << "-----------------------------------------\n";
    for (int i = 0; i < reviewCount; i++)
    {
        cout << "До книги ID: " << reviews[i].bookId
            << " | Користувач: " << reviews[i].userName
            << " | Оцінка: " << reviews[i].rating << "/5\n"
            << "   Відгук: " << reviews[i].text << endl;
        if (i < reviewCount - 1)
        {
            cout << "...\n";
        }
    }
    cout << "-----------------------------------------\n";
}

// =================================================================
//  ОСНОВНА ФУНКЦІЯ MAIN
// =================================================================

int main()
{
    // SetConsoleCP/SetConsoleOutputCP видалено, оскільки вони не потрібні в macOS/Xcode.
    // Xcode працює з UTF-8, що добре підтримує кирилицю.

    Book library[MAX_BOOKS];
    int count = 0;

    Review reviews[MAX_REVIEWS];
    int reviewCount = 0;

    // 1. Завантаження даних при старті
    loadData(library, count, reviews, reviewCount);

    cout << "\n=== АВТОМАТИЗОВАНА СИСТЕМА БІБЛІОТЕКИ ===\n";

    int choice;
    bool running = true;

    while (running)
    {
        cout << "\n======= Головне меню =======\n";
        cout << "1. Додати нову(і) книгу(и)\n";
        cout << "2. Показати всі книги\n";
        cout << "3. Редагувати книгу (за ID)\n";
        cout << "4. Пошук книги\n";
        cout << "5. Додати відгук\n";
        cout << "6. Показати всі відгуки\n";
        cout << "0. Вихід (і збереження даних)\n"; // Змінено опис
        cout << "============================\n";
        cout << "Ваш вибір: ";

        if (!(cin >> choice))
        {
            cout << "Помилка: будь ласка, введіть число.\n";
            clearInput();
            continue;
        }

        switch (choice)
        {
        case 1:
            inputBooks(library, count);
            break;
        case 2:
            displayBooks(library, count);
            break;
        case 3:
            editBook(library, count);
            break;
        case 4:
            searchBooks(library, count);
            break;
        case 5:
            addReview(reviews, reviewCount);
            break;
        case 6:
            displayReviews(reviews, reviewCount);
            break;
        case 0:
            // 2. Збереження даних перед виходом
            saveData(library, count, reviews, reviewCount);
            running = false;
            cout << "\nДані збережено. Роботу завершено.\n";
            break;
        default:
            cout << "\nНевірний вибір. Будь ласка, спробуйте ще раз.\n";
            break;
        }
    }

    return 0;
}
