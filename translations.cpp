#include "translations.h"

TranslationManager& TranslationManager::instance()
{
    static TranslationManager instance;
    return instance;
}

TranslationManager::TranslationManager()
    : currentLanguage("ru")
{
    loadTranslations();
}

void TranslationManager::setLanguage(const QString& language)
{
    if (translations.contains(language) && currentLanguage != language) {
        currentLanguage = language;
        emit languageChanged(language);
    }
}

QString TranslationManager::translate(const QString& key) const
{
    if (translations.contains(currentLanguage)) {
        const QMap<QString, QString>& langTranslations = translations[currentLanguage];
        if (langTranslations.contains(key)) {
            return langTranslations[key];
        }
    }

    // Если перевод не найден, возвращаем ключ
    return key;
}

QStringList TranslationManager::availableLanguages() const
{
    return translations.keys();
}

void TranslationManager::loadTranslations()
{
    // Русский язык
    QMap<QString, QString> ruTranslations;

    // Общие элементы
    ruTranslations["Login"] = "Вход";
    ruTranslations["Password"] = "Пароль";
    ruTranslations["Username"] = "Имя пользователя";
    ruTranslations["Submit"] = "Отправить";
    ruTranslations["Save"] = "Сохранить";
    ruTranslations["Cancel"] = "Отмена";
    ruTranslations["Delete"] = "Удалить";
    ruTranslations["Edit"] = "Редактировать";
    ruTranslations["Add"] = "Добавить";
    ruTranslations["Close"] = "Закрыть";
    ruTranslations["OK"] = "ОК";
    ruTranslations["Yes"] = "Да";
    ruTranslations["No"] = "Нет";

    // Давление
    ruTranslations["Pressure"] = "Давление";
    ruTranslations["Systolic"] = "Систолическое";
    ruTranslations["Diastolic"] = "Диастолическое";
    ruTranslations["Pulse"] = "Пульс";
    ruTranslations["Date"] = "Дата";
    ruTranslations["Time"] = "Время";
    ruTranslations["Comment"] = "Комментарий";
    ruTranslations["Add Entry"] = "Добавить запись";
    ruTranslations["Edit Entry"] = "Редактировать запись";

    // Меню и окна
    ruTranslations["Pressure Tracker"] = "Трекер давления";
    ruTranslations["File"] = "Файл";
    ruTranslations["Settings"] = "Настройки";
    ruTranslations["Language"] = "Язык";
    ruTranslations["Help"] = "Помощь";
    ruTranslations["About"] = "О программе";
    ruTranslations["Support"] = "Поддержка";
    ruTranslations["Account"] = "Аккаунт";
    ruTranslations["Logout"] = "Выйти";

    // Сообщения
    ruTranslations["Operation completed successfully"] = "Операция успешно завершена";
    ruTranslations["Error"] = "Ошибка";
    ruTranslations["Warning"] = "Предупреждение";
    ruTranslations["Information"] = "Информация";
    ruTranslations["Are you sure?"] = "Вы уверены?";
    ruTranslations["Data saved successfully"] = "Данные успешно сохранены";

    // Аутентификация
    ruTranslations["Authorization"] = "Авторизация";
    ruTranslations["Register"] = "Регистрация";
    ruTranslations["Forgot password?"] = "Забыли пароль?";
    ruTranslations["Invalid login or password"] = "Неверный логин или пароль";

    // SupportDialog
    ruTranslations["Support Developer"] = "Поддержка разработчика";
    ruTranslations["If you find this application useful and would like to support its development, you can make a donation to help continue improving the software."] =
        "Если вы находите это приложение полезным и хотели бы поддержать его разработку, вы можете сделать пожертвование, чтобы помочь в дальнейшем улучшении программного обеспечения.";
    ruTranslations["Thank you for your support! ❤️"] = "Спасибо за вашу поддержку! ❤️";

    // MainWindow
    ruTranslations["Current Readings"] = "Текущие показания";
    ruTranslations["Indicators"] = "Индикаторы";
    ruTranslations["Pressure and Pulse Chart"] = "График давления и пульса";
    ruTranslations["Measurement History"] = "История измерений";
    ruTranslations["Add new measurement"] = "Добавить новое измерение";
    ruTranslations["My Account"] = "Мой аккаунт";
    ruTranslations["Support Developer"] = "Поддержка разработчика";
    ruTranslations["Show/Hide Pressure Chart"] = "Показать/скрыть график давления";
    ruTranslations["Show/Hide Pulse Chart"] = "Показать/скрыть график пульса";
    ruTranslations["Days"] = "Дни";
    ruTranslations["Pressure (mm Hg)"] = "Давление (мм рт.ст.)";
    ruTranslations["Pulse (bpm)"] = "Пульс (уд/мин)";
    ruTranslations["Settings Updated"] = "Настройки обновлены";
    ruTranslations["Account settings have been updated successfully."] = "Настройки аккаунта успешно обновлены.";

    // AuthDialog
    ruTranslations["PressureTracker - Authentication"] = "PressureTracker - Аутентификация";
    ruTranslations["Welcome Back"] = "С возвращением";
    ruTranslations["Sign in to your account to continue"] = "Войдите в свой аккаунт чтобы продолжить";
    ruTranslations["Email"] = "Email";
    ruTranslations["your@email.com"] = "ваш@email.com";
    ruTranslations["Enter your password"] = "Введите ваш пароль";
    ruTranslations["Sign In"] = "Войти";
    ruTranslations["Don't have an account? Sign up"] = "Нет аккаунта? Зарегистрироваться";
    ruTranslations["Forgot your password?"] = "Забыли пароль?";
    ruTranslations["Create Account - PressureTracker"] = "Создать аккаунт - PressureTracker";
    ruTranslations["Create Account"] = "Создать аккаунт";
    ruTranslations["Join PressureTracker to start tracking your health"] = "Присоединяйтесь к PressureTracker чтобы начать отслеживать ваше здоровье";
    ruTranslations["Confirm Password"] = "Подтвердите пароль";
    ruTranslations["Create a strong password"] = "Создайте надежный пароль";
    ruTranslations["Confirm your password"] = "Подтвердите ваш пароль";
    ruTranslations["Create Account"] = "Создать аккаунт";
    ruTranslations["Already have an account? Sign in"] = "Уже есть аккаунт? Войти";
    ruTranslations["Invalid Email"] = "Неверный Email";
    ruTranslations["Please enter a valid email address."] = "Пожалуйста, введите корректный email адрес.";
    ruTranslations["Invalid Password"] = "Неверный пароль";
    ruTranslations["Please enter your password."] = "Пожалуйста, введите ваш пароль.";
    ruTranslations["Login Failed"] = "Ошибка входа";
    ruTranslations["Invalid email or password. Please check your credentials and try again."] = "Неверный email или пароль. Пожалуйста, проверьте ваши данные и попробуйте снова.";
    ruTranslations["Weak Password"] = "Слабый пароль";
    ruTranslations["Please choose a stronger password that meets the requirements."] = "Пожалуйста, выберите более надежный пароль, соответствующий требованиям.";
    ruTranslations["Password Mismatch"] = "Пароли не совпадают";
    ruTranslations["Passwords do not match. Please confirm your password."] = "Пароли не совпадают. Пожалуйста, подтвердите ваш пароль.";
    ruTranslations["Registration Successful"] = "Регистрация успешна";
    ruTranslations["Your account has been created successfully!\n\nWelcome to PressureTracker!"] = "Ваш аккаунт успешно создан!\n\nДобро пожаловать в PressureTracker!";
    ruTranslations["• At least 8 characters"] = "• Не менее 8 символов";
    ruTranslations["• At least one uppercase letter"] = "• Хотя бы одна заглавная буква";
    ruTranslations["• At least one lowercase letter"] = "• Хотя бы одна строчная буква";
    ruTranslations["• At least one number"] = "• Хотя бы одна цифра";
    ruTranslations["• At least one special character"] = "• Хотя бы один специальный символ";
    ruTranslations["Very Weak"] = "Очень слабый";
    ruTranslations["Weak"] = "Слабый";
    ruTranslations["Fair"] = "Средний";
    ruTranslations["Good"] = "Хороший";
    ruTranslations["Strong"] = "Надежный";
    ruTranslations["Please contact support at support@pressuretracker.com to reset your password."] = "Пожалуйста, свяжитесь с поддержкой по адресу support@pressuretracker.com для сброса пароля.";

    // AddEntryDialog
    ruTranslations["Add Measurement"] = "Добавить измерение";
    ruTranslations["Date (YYYY-MM-DD)"] = "Дата (ГГГГ-ММ-ДД)";
    ruTranslations["Systolic Pressure"] = "Систолическое давление";
    ruTranslations["Diastolic Pressure"] = "Диастолическое давление";
    ruTranslations["Pulse"] = "Пульс";
    ruTranslations["Save"] = "Сохранить";
    ruTranslations["Cancel"] = "Отмена";
    ruTranslations["Error"] = "Ошибка";
    ruTranslations["Invalid date format. Use YYYY-MM-DD."] = "Неверный формат даты. Используйте ГГГГ-ММ-ДД.";
    ruTranslations["Invalid systolic value."] = "Неверное значение систолического давления.";
    ruTranslations["Invalid diastolic value."] = "Неверное значение диастолического давления.";
    ruTranslations["Invalid pulse value."] = "Неверное значение пульса.";

    // AccountDialog
    ruTranslations["My Account"] = "Мой аккаунт";
    ruTranslations["Personal Information"] = "Личная информация";
    ruTranslations["Preferences"] = "Настройки";
    ruTranslations["First Name:"] = "Имя:";
    ruTranslations["Last Name:"] = "Фамилия:";
    ruTranslations["Email:"] = "Email:";
    ruTranslations["Date of Birth:"] = "Дата рождения:";
    ruTranslations["Language:"] = "Язык:";
    ruTranslations["English"] = "Английский";
    ruTranslations["Russian"] = "Русский";
    ruTranslations["Spanish"] = "Испанский";
    ruTranslations["French"] = "Французский";
    ruTranslations["Save Settings"] = "Сохранить настройки";
    ruTranslations["Log Out"] = "Выйти";
    ruTranslations["Change Photo"] = "Изменить фото";
    ruTranslations["Success"] = "Успех";
    ruTranslations["Settings saved successfully.\nSome changes may require restart to take effect."] =
        "Настройки успешно сохранены.\nНекоторые изменения могут потребовать перезапуска для применения.";
    ruTranslations["Log Out"] = "Выйти";
    ruTranslations["Are you sure you want to log out?"] = "Вы уверены, что хотите выйти?";
    ruTranslations["Logged Out"] = "Выход выполнен";
    ruTranslations["You have been successfully logged out."] = "Вы успешно вышли из системы.";
    ruTranslations["Photo change functionality would be implemented here."] = "Функция смены фотографии будет реализована здесь.";
    ruTranslations["Current Password:"] = "Текущий пароль:";
    ruTranslations["New Password:"] = "Новый пароль:";
    ruTranslations["Confirm Password:"] = "Подтвердите пароль:";
    ruTranslations["Change Password"] = "Сменить пароль";

    translations["ru"] = ruTranslations;

    // Английский язык (по умолчанию)
    QMap<QString, QString> enTranslations;

    // Общие элементы
    enTranslations["Login"] = "Login";
    enTranslations["Password"] = "Password";
    enTranslations["Username"] = "Username";
    enTranslations["Submit"] = "Submit";
    enTranslations["Save"] = "Save";
    enTranslations["Cancel"] = "Cancel";
    enTranslations["Delete"] = "Delete";
    enTranslations["Edit"] = "Edit";
    enTranslations["Add"] = "Add";
    enTranslations["Close"] = "Close";
    enTranslations["OK"] = "OK";
    enTranslations["Yes"] = "Yes";
    enTranslations["No"] = "No";

    // Давление
    enTranslations["Pressure"] = "Pressure";
    enTranslations["Systolic"] = "Systolic";
    enTranslations["Diastolic"] = "Diastolic";
    enTranslations["Pulse"] = "Pulse";
    enTranslations["Date"] = "Date";
    enTranslations["Time"] = "Time";
    enTranslations["Comment"] = "Comment";
    enTranslations["Add Entry"] = "Add Entry";
    enTranslations["Edit Entry"] = "Edit Entry";

    // Меню и окна
    enTranslations["Pressure Tracker"] = "Pressure Tracker";
    enTranslations["File"] = "File";
    enTranslations["Settings"] = "Settings";
    enTranslations["Language"] = "Language";
    enTranslations["Help"] = "Help";
    enTranslations["About"] = "About";
    enTranslations["Support"] = "Support";
    enTranslations["Account"] = "Account";
    enTranslations["Logout"] = "Logout";

    // Сообщения
    enTranslations["Operation completed successfully"] = "Operation completed successfully";
    enTranslations["Error"] = "Error";
    enTranslations["Warning"] = "Warning";
    enTranslations["Information"] = "Information";
    enTranslations["Are you sure?"] = "Are you sure?";
    enTranslations["Data saved successfully"] = "Data saved successfully";

    // Аутентификация
    enTranslations["Authorization"] = "Authorization";
    enTranslations["Register"] = "Register";
    enTranslations["Forgot password?"] = "Forgot password?";
    enTranslations["Invalid login or password"] = "Invalid login or password";

    // SupportDialog
    enTranslations["Support Developer"] = "Support Developer";
    enTranslations["If you find this application useful and would like to support its development, you can make a donation to help continue improving the software."] =
        "If you find this application useful and would like to support its development, you can make a donation to help continue improving the software.";
    enTranslations["Thank you for your support! ❤️"] = "Thank you for your support! ❤️";

    // MainWindow
    enTranslations["Current Readings"] = "Current Readings";
    enTranslations["Indicators"] = "Indicators";
    enTranslations["Pressure and Pulse Chart"] = "Pressure and Pulse Chart";
    enTranslations["Measurement History"] = "Measurement History";
    enTranslations["Add new measurement"] = "Add new measurement";
    enTranslations["My Account"] = "My Account";
    enTranslations["Support Developer"] = "Support Developer";
    enTranslations["Show/Hide Pressure Chart"] = "Show/Hide Pressure Chart";
    enTranslations["Show/Hide Pulse Chart"] = "Show/Hide Pulse Chart";
    enTranslations["Days"] = "Days";
    enTranslations["Pressure (mm Hg)"] = "Pressure (mm Hg)";
    enTranslations["Pulse (bpm)"] = "Pulse (bpm)";
    enTranslations["Settings Updated"] = "Settings Updated";
    enTranslations["Account settings have been updated successfully."] = "Account settings have been updated successfully.";

    // AuthDialog
    enTranslations["PressureTracker - Authentication"] = "PressureTracker - Authentication";
    enTranslations["Welcome Back"] = "Welcome Back";
    enTranslations["Sign in to your account to continue"] = "Sign in to your account to continue";
    enTranslations["Email"] = "Email";
    enTranslations["your@email.com"] = "your@email.com";
    enTranslations["Enter your password"] = "Enter your password";
    enTranslations["Sign In"] = "Sign In";
    enTranslations["Don't have an account? Sign up"] = "Don't have an account? Sign up";
    enTranslations["Forgot your password?"] = "Forgot your password?";
    enTranslations["Create Account - PressureTracker"] = "Create Account - PressureTracker";
    enTranslations["Create Account"] = "Create Account";
    enTranslations["Join PressureTracker to start tracking your health"] = "Join PressureTracker to start tracking your health";
    enTranslations["Confirm Password"] = "Confirm Password";
    enTranslations["Create a strong password"] = "Create a strong password";
    enTranslations["Confirm your password"] = "Confirm your password";
    enTranslations["Create Account"] = "Create Account";
    enTranslations["Already have an account? Sign in"] = "Already have an account? Sign in";
    enTranslations["Invalid Email"] = "Invalid Email";
    enTranslations["Please enter a valid email address."] = "Please enter a valid email address.";
    enTranslations["Invalid Password"] = "Invalid Password";
    enTranslations["Please enter your password."] = "Please enter your password.";
    enTranslations["Login Failed"] = "Login Failed";
    enTranslations["Invalid email or password. Please check your credentials and try again."] = "Invalid email or password. Please check your credentials and try again.";
    enTranslations["Weak Password"] = "Weak Password";
    enTranslations["Please choose a stronger password that meets the requirements."] = "Please choose a stronger password that meets the requirements.";
    enTranslations["Password Mismatch"] = "Password Mismatch";
    enTranslations["Passwords do not match. Please confirm your password."] = "Passwords do not match. Please confirm your password.";
    enTranslations["Registration Successful"] = "Registration Successful";
    enTranslations["Your account has been created successfully!\n\nWelcome to PressureTracker!"] = "Your account has been created successfully!\n\nWelcome to PressureTracker!";
    enTranslations["• At least 8 characters"] = "• At least 8 characters";
    enTranslations["• At least one uppercase letter"] = "• At least one uppercase letter";
    enTranslations["• At least one lowercase letter"] = "• At least one lowercase letter";
    enTranslations["• At least one number"] = "• At least one number";
    enTranslations["• At least one special character"] = "• At least one special character";
    enTranslations["Very Weak"] = "Very Weak";
    enTranslations["Weak"] = "Weak";
    enTranslations["Fair"] = "Fair";
    enTranslations["Good"] = "Good";
    enTranslations["Strong"] = "Strong";
    enTranslations["Please contact support at support@pressuretracker.com to reset your password."] = "Please contact support at support@pressuretracker.com to reset your password.";

    // AddEntryDialog
    enTranslations["Add Measurement"] = "Add Measurement";
    enTranslations["Date (YYYY-MM-DD)"] = "Date (YYYY-MM-DD)";
    enTranslations["Systolic Pressure"] = "Systolic Pressure";
    enTranslations["Diastolic Pressure"] = "Diastolic Pressure";
    enTranslations["Pulse"] = "Pulse";
    enTranslations["Save"] = "Save";
    enTranslations["Cancel"] = "Cancel";
    enTranslations["Error"] = "Error";
    enTranslations["Invalid date format. Use YYYY-MM-DD."] = "Invalid date format. Use YYYY-MM-DD.";
    enTranslations["Invalid systolic value."] = "Invalid systolic value.";
    enTranslations["Invalid diastolic value."] = "Invalid diastolic value.";
    enTranslations["Invalid pulse value."] = "Invalid pulse value.";

    // AccountDialog
    enTranslations["My Account"] = "My Account";
    enTranslations["Personal Information"] = "Personal Information";
    enTranslations["Preferences"] = "Preferences";
    enTranslations["First Name:"] = "First Name:";
    enTranslations["Last Name:"] = "Last Name:";
    enTranslations["Email:"] = "Email:";
    enTranslations["Date of Birth:"] = "Date of Birth:";
    enTranslations["Language:"] = "Language:";
    enTranslations["English"] = "English";
    enTranslations["Russian"] = "Russian";
    enTranslations["Save Settings"] = "Save Settings";
    enTranslations["Log Out"] = "Log Out";
    enTranslations["Change Photo"] = "Change Photo";
    enTranslations["Success"] = "Success";
    enTranslations["Settings saved successfully.\nSome changes may require restart to take effect."] =
        "Settings saved successfully.\nSome changes may require restart to take effect.";
    enTranslations["Log Out"] = "Log Out";
    enTranslations["Are you sure you want to log out?"] = "Are you sure you want to log out?";
    enTranslations["Logged Out"] = "Logged Out";
    enTranslations["You have been successfully logged out."] = "You have been successfully logged out.";
    enTranslations["Photo change functionality would be implemented here."] = "Photo change functionality would be implemented here.";
    enTranslations["Current Password:"] = "Current Password:";
    enTranslations["New Password:"] = "New Password:";
    enTranslations["Confirm Password:"] = "Confirm Password:";
    enTranslations["Change Password"] = "Change Password";

    translations["en"] = enTranslations;
}
