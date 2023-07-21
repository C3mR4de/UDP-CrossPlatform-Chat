#include <SFML/Graphics.hpp> // Графическая библиотека
#include <SFML/Network.hpp> // Сетевая библиотека
#include <iostream> // Библиотека ввода-вывода
#include <list> // Двусвязные списки

// Legacy
/*
#ifdef _WIN32
    #include <windows.h> // Библиотека Windows (для локалей)
#endif
*/

#define SERVER_PORT 9093 // Порт сервера
#define CLIENT_PORT 9094 // Порт клиента

namespace UdpMessenger
{
      // Тип подключения (перечисляемый тип)
      enum ConnectionType
      {
            None = -1, // Не определён
            Client, // Клиент
            Server // Сервер
      };

      // Тип подключения (переменная)
      ConnectionType ConType = ConnectionType::None;

      // Класс пользователя
      class User
      {
      public:

            sf::String Nickname; // Никнейм пользователя
            sf::IpAddress Address; // IP-адрес пользователя
            bool IsOnline; // В сети ли пользователь

            // Конструктор с параметрами
            User(sf::String Nickname, sf::IpAddress Address)
            {
                  this->Nickname = Nickname; // Никнейм
                  this->Address = Address; // Адрес
                  IsOnline = true; // Онлайн
            }
      };

      // Класс UDP-сервера
      class UdpServer
      {
            sf::UdpSocket Socket; // Сокет
            sf::IpAddress LastClientAddress; // IP-адрес последнего клиента
            sf::Uint16 SendPort; // Порт отправки
            sf::Uint16 ReceivePort; // Порт получения

      public:

            sf::Packet SendPacket; // Пакет отправки
            sf::Packet ReceivePacket; // Пакет получения

            std::list<User> Users; // Пользователи

            // Конструктор с параметрами
            UdpServer(sf::Uint16 SendPort, sf::Uint16 ReceivePort)
            {
                  this->SendPort = SendPort; // Порт отправки
                  this->ReceivePort = ReceivePort; // Порт получения
                  Socket.setBlocking(false); // Отключаем блокировку сокета
                  Socket.bind(ReceivePort); // Биндим на порт получения
            }

            // Разослать данные пользователям
            void Send()
            {
                  // Перебираем каждого пользователя
                  for (auto Receiver = Users.cbegin(); Receiver != Users.cend(); Receiver++)
                  {
                        // Если он онлайн, то высылаем ему данные
                        if (Receiver->IsOnline)
                              Socket.send(SendPacket, Receiver->Address, SendPort);
                  }
            }

            // Получать данные от пользователей
            inline sf::Socket::Status Receive()
            {
                  return Socket.receive(ReceivePacket, LastClientAddress, ReceivePort); // Пытаемся получить данные
            }

            // IP-адрес последнего клиента
            inline sf::IpAddress LastClient()
            {
                  return LastClientAddress;
            }

            // Включение/отключение блокировки
            void SetBlocking(bool Blocking)
            {
                  Socket.setBlocking(Blocking);
            }

            // Деструктор
            ~UdpServer()
            {
                  Socket.unbind();
            }
      };

      // Класс UDP-клиента
      class UdpClient
      {
            sf::UdpSocket Socket; // Сокет
            sf::IpAddress SendServerAddress; // IP-адрес сервера для отправки
            sf::IpAddress ReceiveServerAddress; // IP-адрес сервера для получения
            sf::Uint16 SendPort; // Порт отправки
            sf::Uint16 ReceivePort; // Порт получения

      public:

            sf::Packet SendPacket; // Пакет отправки
            sf::Packet ReceivePacket; // Пакет получения

            // Конструктор с параметрами
            UdpClient(sf::IpAddress ServerAddress, sf::Uint16 SendPort, sf::Uint16 ReceivePort)
            {
                  SendServerAddress = ServerAddress; // Адрес сервера
                  this->SendPort = SendPort; // Порт отправки
                  this->ReceivePort = ReceivePort; // Порт получения
                  Socket.setBlocking(false);
                  Socket.bind(ReceivePort);
            }

            // Отослать данные серверу
            void Send()
            {
                  Socket.send(SendPacket, SendServerAddress, SendPort);
            }

            // Получать данные от сервера
            inline sf::Socket::Status Receive()
            {
                  return Socket.receive(ReceivePacket, ReceiveServerAddress, ReceivePort);
            }

            // Включение/отключение блокировки
            void SetBlocking(bool Blocking)
            {
                  Socket.setBlocking(Blocking);
            }

            // Деструктор
            ~UdpClient()
            {
                  Socket.unbind();
            }
      };
}

int main()
{
     // Legacy вход в систему
     /*
      #ifdef _WIN32
          SetConsoleCP(CP_UTF8); // Задаём кодировку UTF-8 для ввода
          SetConsoleOutputCP(CP_UTF8); // Задаём кодировку UTF-8 для вывода
      #endif

      sf::Font Font; // Шрифт
      Font.loadFromFile("C:/Windows/Fonts/comic.ttf"); // Загрузка шрифта

      bool Type; // Тип подключения (сервер или клиент)
      sf::String Nickname; // Никнейм

      std::cout << "Введите ваш никнейм: "; // Ввод никнейма (пока что из консоли)
      {
            std::string NicknameBuf;
            getline(std::cin, NicknameBuf);
            // Nickname = sf::String::fromUtf8(NicknameBuf.begin(), NicknameBuf.end());
            Nickname = NicknameBuf;
      }

      std::cout << "Введите тип подключения (0 - клиент, 1 - сервер): "; // Запрос на ввод типа подключения
      std::cin >> Type; // Ввод

      */

      sf::RenderWindow NicknameWindow(sf::VideoMode::getDesktopMode(), L"SFML UDP Messenger", sf::Style::Default);
      NicknameWindow.setSize(sf::Vector2u(NicknameWindow.getDefaultView().getCenter().x, NicknameWindow.getDefaultView().getCenter().y));

      sf::Event Event;

      sf::Font Font; // Шрифт
      Font.loadFromFile("C:/Windows/Fonts/comic.ttf");

      sf::Text NicknameText, TypeText;

      NicknameText.setFont(Font);
      TypeText.setFont(Font);

      sf::String NicknameString, TypeString; // Никнейм
      bool Type; // Тип подключения (сервер или клиент)

      while (NicknameWindow.isOpen())
      {
            // Отслеживание одиночных событий
            while (NicknameWindow.pollEvent(Event))
            {
                  // Перебор одиночных событий
                  switch (Event.type)
                  {
                        // Если нажата кнопка "Закрыть", то закрываем
                        case sf::Event::EventType::Closed:
                              NicknameWindow.close();
                              return 0;
                              break;

                        // Если введён текст
                        case sf::Event::EventType::TextEntered:

                              // Если не нажат Ctrl (иначе на экран вводятся квадраты)
                              if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl))
                              {
                                    // Если нажат не Backspace, то добавляем соответствующий символ, иначе удаляем последний символ
                                    if (Event.text.unicode != 8 && Event.text.unicode != 13)
                                          NicknameString += (wchar_t)Event.text.unicode;
                                    else
                                    {
                                          if (Event.text.unicode == 8)
                                                if (NicknameString.getSize() > 0)
                                                      NicknameString.erase(NicknameString.getSize() - 1);
                                    }
                              }

                              break;

                        // Остальное явно скипаем, дабы избежать варнинга
                        default:
                              break;
                  }
            }

            // Если нажата клавиша "Enter" и при этом поле ввода сообщения не пустое
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && !NicknameString.isEmpty())
                  NicknameWindow.close();

            // Обновляем форматирование авторизационного текста
            NicknameText.setString(L"Введите ваш никнейм: " + NicknameString);

            // Отрисовка текста на экран
            NicknameWindow.clear();
            NicknameWindow.draw(NicknameText);
            NicknameWindow.display();
      }

      sf::RenderWindow TypeWindow(sf::VideoMode::getDesktopMode(), L"SFML UDP Messenger", sf::Style::Default);
      TypeWindow.setSize(sf::Vector2u(TypeWindow.getDefaultView().getCenter().x, TypeWindow.getDefaultView().getCenter().y));

      while (TypeWindow.isOpen())
      {
            // Отслеживание одиночных событий
            while (TypeWindow.pollEvent(Event))
            {
                  // Перебор одиночных событий
                  switch (Event.type)
                  {
                        // Если нажата кнопка "Закрыть", то закрываем
                        case sf::Event::EventType::Closed:
                              TypeWindow.close();
                              return 0;
                              break;

                        // Если введён текст
                        case sf::Event::EventType::TextEntered:

                              // Если не нажат Ctrl (иначе на экран вводятся квадраты)
                              if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl))
                              {
                                    // Если нажат не Backspace, то добавляем соответствующий символ, иначе удаляем последний символ
                                    if (Event.text.unicode != 8 && Event.text.unicode != 13)
                                          TypeString += (wchar_t)Event.text.unicode;
                                    else
                                    {
                                          if (Event.text.unicode == 8)
                                                if (TypeString.getSize() > 0)
                                                      TypeString.erase(TypeString.getSize() - 1);
                                    }
                              }

                              break;

                        // Остальное явно скипаем, дабы избежать варнинга
                        default:
                              break;
                  }
            }

            // Если нажата клавиша "Enter" и при этом поле ввода сообщения не пустое
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && (TypeString == L"0" || TypeString == L"1"))
            {
                  Type = (TypeString == L"1");
                  TypeWindow.close();
            }

            // Обновляем форматирование авторизационного текста
            TypeText.setString(L"Введите тип подключения (0 - клиент, 1 - сервер): " + TypeString);

            // Отрисовка текста на экран
            TypeWindow.clear();
            TypeWindow.draw(TypeText);
            TypeWindow.display();
      }

      // Задаём тип подключения в зависимости от того, что ввёл пользователь
      Type ? UdpMessenger::ConType = UdpMessenger::ConnectionType::Server : UdpMessenger::ConType = UdpMessenger::ConnectionType::Client;

      // Проверяем тип подключения
      switch (UdpMessenger::ConType)
      {
            // Серверный тип
            case UdpMessenger::ConnectionType::Server:
                  {
                        // Создаём UDP-сервер
                        UdpMessenger::UdpServer Server(CLIENT_PORT, SERVER_PORT);

                        sf::String HistoryString = L"Приём сообщений на порт " + sf::String(std::to_string(SERVER_PORT)) + L"...\n"; // Строка истории сообщений
                        sf::Text HistoryText; // Текст истории сообщений

                        sf::String MessageString; // Строка истории сообщений
                        sf::Text MessageText; // Текст истории сообщений

                        HistoryText.setFont(Font); // Задаём шрифт для текста истории
                        MessageText.setFont(Font); // И для текста сообщения тоже

                        // Создаём окно
                        sf::RenderWindow Window(sf::VideoMode::getDesktopMode(), L"SFML UDP Messenger", sf::Style::Default);

                        Window.setSize(sf::Vector2u(Window.getDefaultView().getCenter().x, Window.getDefaultView().getCenter().y));

                        // Событие
                        sf::Event Event;

                        // Задаём позицию для поля ввода сообщения
                        MessageText.setPosition(0, Window.getDefaultView().getSize().y);

                        // Главный цикл
                        while (Window.isOpen())
                        {
                              // Отслеживание одиночных событий
                              while (Window.pollEvent(Event))
                              {
                                    // Перебор одиночных событий
                                    switch (Event.type)
                                    {
                                          // Если нажата кнопка "Закрыть", то закрываем
                                          case sf::Event::EventType::Closed:
                                                Window.close();
                                                break;

                                          // Если введён текст
                                          case sf::Event::EventType::TextEntered:

                                                // Если не нажат Ctrl (иначе на экран вводятся квадраты)
                                                if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl))
                                                {
                                                      // Если нажат не Backspace, то добавляем соответствующий символ, иначе удаляем последний символ
                                                      if (Event.text.unicode != 8 && Event.text.unicode != 13)
                                                            MessageString += (wchar_t)Event.text.unicode;
                                                      else
                                                      {
                                                            if (Event.text.unicode == 8)
                                                                  if (MessageString.getSize() > 0)
                                                                        MessageString.erase(MessageString.getSize() - 1);
                                                      }
                                                }

                                                break;

                                          // Остальное явно скипаем, дабы избежать варнинга
                                          default:
                                                break;
                                    }
                              }

                              // Принимаем данные с сокета
                              switch (Server.Receive())
                              {
                                    // Если что-то пришло успешно
                                    case sf::Socket::Status::Done:
                                          {
                                                // Создаём буфер для данных
                                                sf::String Command, Buffer;

                                                // Считываем принятые данные
                                                Server.ReceivePacket >> Command >> Buffer;
                                                //std::cout << Command.toAnsiString() << " " << Buffer.toAnsiString() << "\n";

                                                // Если пришло сообщение
                                                if (Command == L"msg")
                                                {
                                                      // Перебираем всех подключённых клиентов
                                                      for (auto Sender = Server.Users.cbegin(); Sender != Server.Users.cend(); Sender++)
                                                      {
                                                            // Если среди них находится тот, кого мы ищем
                                                            if (Sender->Address == Server.LastClient())
                                                            {
                                                                  HistoryString += Sender->Nickname + L": " + Buffer + L"\n"; // Добавляем его сообщение к истории

                                                                  Server.SendPacket << L"hst" << Sender->Nickname + L": " + Buffer + L"\n"; // Подготавливаем пакет к рассылке
                                                                  Server.Send(); // Отсылаем каждому из наших клиентов пакет
                                                                  Server.SendPacket.clear(); // Очищаем пакет

                                                                  break; // Выходим из цикла
                                                           }
                                                      }
                                                }
                                                else if (Command == L"con") // Если кто-то отправил запрос о подключении
                                                {
                                                      // Вспомогательная переменная для сохранения
                                                      bool Existed = false;

                                                      // Перебираем всех подключённых клиентов
                                                      for (auto Sender = Server.Users.begin(); Sender != Server.Users.end(); Sender++)
                                                      {
                                                            // Если среди них находится наш клиент
                                                            if (Sender->Address == Server.LastClient())
                                                            {
                                                                  // Если он не в сети (ранее подключался и выходил)
                                                                  if (!Sender->IsOnline)
                                                                  {
                                                                        Sender->Nickname = Buffer; // Задаём ему новый ник (а мало ли он его изменил?)
                                                                        Sender->IsOnline = true; // И говорим, что он теперь онлайн
                                                                        Existed = true; // И да, он ранее существовал (уже заходил)
                                                                        HistoryString += Sender->Nickname + L" подключился к серверу\n"; // Говорим, что он вновь подключился к серверу

                                                                        Server.SendPacket << L"con" << L"accept"; // Подготавливаем ответ (положительный)
                                                                        Server.Send(); // Отсылаем
                                                                        Server.SendPacket.clear(); // Очищаем пакет

                                                                        Server.SendPacket << L"hst" << Sender->Nickname + L" подключился к серверу\n"; // Подготавливаем пакет к рассылке
                                                                        Server.Send(); // Отсылаем каждому из них наш пакет
                                                                        Server.SendPacket.clear(); // Очищаем пакет

                                                                        break; // Выходим из цикла
                                                                  }
                                                            }
                                                      }

                                                      // Если он ранее не существовал
                                                      if (!Existed)
                                                      {
                                                            Server.SendPacket << L"hst" << Buffer + L" подключился к серверу\n"; // Подготавливаем пакет к рассылке
                                                            Server.Send(); // Отсылаем каждому из них наш пакет
                                                            Server.SendPacket.clear(); // Очищаем пакет

                                                            Server.Users.push_back(UdpMessenger::User(Buffer, Server.LastClient())); // Добавляем его в список пользователей
                                                            HistoryString += Buffer + L" подключился к серверу\n"; // И выводим сообщение

                                                            Server.SendPacket << L"con" << L"accept"; // Подготавливаем ответ (положительный)
                                                            Server.Send(); // Отсылаем
                                                            Server.SendPacket.clear(); // Очищаем пакет
                                                      }
                                                }
                                                else if (Command == L"dsc") // Если кто-то отключился
                                                {
                                                      // Перебираем всех существующих пользователей
                                                      for (auto Sender = Server.Users.begin(); Sender != Server.Users.end(); Sender++)
                                                      {
                                                            // Если среди них находится наш клиент
                                                            if (Sender->Address == Server.LastClient())
                                                            {
                                                                  Sender->IsOnline = false; // Значит он больше не в сети
                                                                  HistoryString += Sender->Nickname + L" отключился от сервера\n"; // Выводим сообщение об этом

                                                                  Server.SendPacket << L"hst" << Sender->Nickname + L" отключился от сервера\n"; // Подготавливаем пакет к рассылке
                                                                  Server.Send(); // Отсылаем пакет всем клиентам (историю сообщений)
                                                                  Server.SendPacket.clear(); // Очищаем пакет

                                                                  break; // Выходим из цикла
                                                            }
                                                      }
                                                }

                                                Server.ReceivePacket.clear(); // Очищаем пакет принятия данных
                                          }
                                          break;

                                    default:
                                          break;
                              }

                              // Если нажата клавиша "Enter" и при этом поле ввода сообщения не пустое
                              if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && !MessageString.isEmpty())
                              {
                                    HistoryString += NicknameString + L": " + MessageString + L"\n"; // Выводим сообщение

                                    Server.SendPacket << L"hst" << NicknameString + L": " + MessageString + L"\n"; // Подготавливаем пакет к рассылке
                                    Server.Send(); // Рассылаем всем клиентам
                                    Server.SendPacket.clear(); // Очищаем пакет

                                    MessageString.clear(); // Очищаем поле ввода сообщения
                              }

                              // Обновляем значение нашего текста и его форматирование
                              HistoryText.setString(HistoryString);
                              HistoryText.setOrigin(0, HistoryText.getGlobalBounds().height + 20);
                              HistoryText.setPosition(0,  Window.getDefaultView().getSize().y - MessageText.getCharacterSize());

                              // Обновляем форматирование сообщения
                              MessageText.setString(L"Введите сообщение: " + MessageString);
                              MessageText.setOrigin(0, MessageText.getCharacterSize() + 10);

                              // Отрисовка текста на экран
                              Window.clear();
                              Window.draw(HistoryText);
                              Window.draw(MessageText);
                              Window.display();
                        }

                        Server.SendPacket << L"dsc" << NicknameString; // Подготавливаем запрос к подключению
                        Server.Send(); // Посылаем запрос
                        Server.SendPacket.clear(); // Очищаем пакет
                  }
                  break;

            // Клиентский тип
            case UdpMessenger::ConnectionType::Client:
                  {
                        std::string ServerAddress; // Строка для хранения введённого IP-адреса сервера
                        std::cout << "Введите IP-адрес сервера: "; // Сообщение

                        std::cin.get(); // Это небольшой костыль, который связан с багом функции getline
                        getline(std::cin, ServerAddress); // Непосредственно сам ввод IP-адреса

                        // Создаём UDP-клиент
                        UdpMessenger::UdpClient Client(sf::IpAddress(ServerAddress), SERVER_PORT, CLIENT_PORT);

                        sf::String HistoryString = L"Подключение к серверу " + sf::String(ServerAddress) + L"...\n"; // Строка истории сообщений
                        sf::Text HistoryText; // Текст истории сообщений

                        sf::String MessageString; // Строка истории сообщений
                        sf::Text MessageText; // Текст истории сообщений

                        HistoryText.setFont(Font); // Задаём шрифт для текста истории
                        MessageText.setFont(Font); // И для текста сообщения тоже

                        Client.SendPacket << L"con" << NicknameString; // Подготавливаем запрос к подключению
                        Client.Send(); // Посылаем запрос
                        Client.SendPacket.clear(); // Очищаем пакет

                        std::cout << "Ожидание ответа сервера..."; // Выводим текст ожидания

                        Client.SetBlocking(true); // Включаем блокировку сокета

                        auto Connection = Client.Receive(); // Приём данных

                        if (Connection == sf::Socket::Status::Done) // Ждём, пока сервер отправит данные
                        {
                              sf::String ConCommand, ConBuffer; // Создаём две переменные (буфера)

                              Client.ReceivePacket >> ConCommand >> ConBuffer; // Считываем в строки

                              //std::cout << ConCommand.toAnsiString() << " " << ConBuffer.toAnsiString() << "\n";

                              if (ConCommand == L"con" && ConBuffer == L"accept") // Если пришло то, что нужно
                              {
                                    Client.ReceivePacket.clear(); // Очищаем пакет приёма

                                    Client.SetBlocking(false); // Отключаем блокировку сокета

                                    // Создаём окно
                                    sf::RenderWindow Window(sf::VideoMode::getDesktopMode(), L"SFML UDP Messenger", sf::Style::Default);

                                    Window.setSize(sf::Vector2u(Window.getDefaultView().getCenter().x, Window.getDefaultView().getCenter().y));

                                    // Событие
                                    sf::Event Event;

                                    // Задаём позицию для поля ввода сообщения
                                    MessageText.setPosition(0, Window.getDefaultView().getSize().y);

                                    // Главный цикл
                                    while (Window.isOpen())
                                    {
                                          // Отслеживание одиночных событий
                                          while (Window.pollEvent(Event))
                                          {
                                                // Перебор одиночных событий
                                                switch (Event.type)
                                                {
                                                      // Если нажата кнопка "Закрыть", то закрываем
                                                      case sf::Event::EventType::Closed:
                                                            Window.close();
                                                            break;

                                                      // Если введён текст
                                                      case sf::Event::EventType::TextEntered:

                                                                  // Если не нажат Ctrl (иначе на экран вводятся квадраты)
                                                                  if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl))
                                                                  {
                                                                        // Если нажат не Backspace, то добавляем соответствующий символ, иначе удаляем последний символ
                                                                        if (Event.text.unicode != 8 && Event.text.unicode != 13)
                                                                              MessageString += (wchar_t)Event.text.unicode;
                                                                        else
                                                                        {
                                                                              if (Event.text.unicode == 8)
                                                                                    if (MessageString.getSize() > 0)
                                                                                          MessageString.erase(MessageString.getSize() - 1);
                                                                        }
                                                                  }

                                                            break;

                                                      // Остальное явно скипаем, дабы избежать варнинга
                                                      default:
                                                            break;
                                                }
                                          }

                                          // Принимаем данные с сокета
                                          switch (Client.Receive())
                                          {
                                                // Если данные приняты успешно
                                                case sf::Socket::Status::Done:
                                                      {
                                                            // Создаём две буферные переменные
                                                            sf::String Command, Buffer;

                                                            // Сохраням в них то, что пришло
                                                            Client.ReceivePacket >> Command >> Buffer;

                                                            //std::cout << Command.toAnsiString() << " " << Buffer.toAnsiString() << "\n";

                                                            // Если пришла часть истории сообщений
                                                            if (Command == L"hst")
                                                            {
                                                                  HistoryString += Buffer; // Записываем её в строку
                                                            }
                                                            else if (Command == L"dsc")
                                                            {
                                                                  Window.close(); // Тупо закрываем окно ахах
                                                            }

                                                            // Очищаем пакет
                                                            Client.ReceivePacket.clear();
                                                      }
                                                      break;

                                                default:
                                                      break;
                                          }

                                          // Если нажата клавиша "Enter" и при этом поле ввода сообщения не пустое
                                          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && !MessageString.isEmpty())
                                          {
                                                //HistoryString += Nickname + L": " + MessageString + L"\n"; // Выводим сообщение
                                                Client.SendPacket << L"msg" << MessageString; // Подготавливаем пакет к рассылке
                                                Client.Send(); // Отсылаем серверу
                                                Client.SendPacket.clear(); // Очищаем пакет

                                                MessageString.clear(); // Очищаем поле ввода сообщения
                                          }

                                          // Обновляем значение нашего текста и его форматирование
                                          HistoryText.setString(HistoryString);
                                          HistoryText.setOrigin(0, HistoryText.getGlobalBounds().height + 20);
                                          HistoryText.setPosition(0,  Window.getDefaultView().getSize().y - MessageText.getCharacterSize());

                                          // Обновляем форматирование сообщения
                                          MessageText.setString(L"Введите сообщение: " + MessageString);
                                          MessageText.setOrigin(0, MessageText.getCharacterSize() + 10);

                                          // Отрисовка текста на экран
                                          Window.clear();
                                          Window.draw(HistoryText);
                                          Window.draw(MessageText);
                                          Window.display();
                                    }

                                    Client.SendPacket << L"dsc" << NicknameString; // Подготавливаем запрос к подключению
                                    Client.Send(); // Посылаем запрос
                                    Client.SendPacket.clear(); // Очищаем пакет
                              }
                        }
                  }
                  break;

            default:
                  break;
      }

    return 0;
}
