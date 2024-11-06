# Tema-robo2
1. Descrierea Proiectului
Scopul acestui proiect este de a dezvolta un joc de tip TypeRacer folosind un Arduino UNO, un LED RGB, două butoane și alte componente de bază. Jucătorul va trebui să tasteze corect cuvintele afișate într-un interval de timp, iar dificultatea jocului poate fi ajustată între trei nivele. LED-ul RGB va indica diferite stări ale jocului, precum repaus, eroare la tastare, sau răspuns corect.

2. Componente Utilizate
Arduino UNO - Placă de dezvoltare care controlează întregul joc. ATmega328P
LED RGB - Indicator vizual pentru starea jocului (corect/greșit/repaus).
Buton Start/Stop - Inițiază și oprește o rundă de joc.
Buton de Dificultate - Permite schimbarea dificultății între Easy, Medium și Hard.
Rezistoare:
3x rezistențe de 220/330 ohmi pentru LED RGB
2x rezistențe de 1000 ohmi pentru butoane
Breadboard și fire de legătură - Pentru conectarea componentelor.
3. Cerințe Tehnice
Indicator Stare (LED RGB):

Repaus: LED-ul afișează culoarea albă.
Start Joc: LED-ul clipește în alb timp de 3 secunde pentru a indica numărătoarea inversă până la începerea rundei , dupa care se face verde .
Răspuns greșit: LED-ul devine roșu.
Butonul Start/Stop:

În Repaus: La apăsare, inițiază o rundă nouă după o numărătoare inversă de 3 secunde.
În Joc: La apăsare, oprește runda curentă imediat.
Butonul de Dificultate:

Poate fi utilizat doar în starea de repaus.
Schimbă dificultatea jocului (Easy, Medium, Hard).
La fiecare apăsare, trimite un mesaj în terminal: „Easy/Medium/Hard mode on!”.
Generarea Cuvintelor:

Cuvintele sunt afișate aleatoriu în timpul unei runde.
Dacă un cuvânt este scris corect, următorul cuvânt apare imediat.
În caz de greșeală, LED-ul devine roșu și un alt cuvânt va apărea după un interval de timp specific dificultății.
Limitarea de Timp:

Fiecare rundă are o durată maximă de 30 de secunde.
La finalul rundei, se afișează scorul total (numărul de cuvinte scrise corect).

Shema electrica :
![Screenshot 2024-11-06 235043](https://github.com/user-attachments/assets/a2cb24f5-2f75-4567-80ce-f80a40c5bdbb)

Poze cu setup-ul fizic :
![WhatsApp Image 2024-11-06 at 23 49 59_663fb024](https://github.com/user-attachments/assets/a411a295-65e5-46fa-911b-359e6b75387b)
![WhatsApp Image 2024-11-06 at 23 50 01_e8b34e3a](https://github.com/user-attachments/assets/60415863-d22b-4b7f-a530-b78985f7f1fe)
![WhatsApp Image 2024-11-06 at 23 50 02_a0112641](https://github.com/user-attachments/assets/4c6ba226-ab85-4076-a653-be257f7f28a3)


VIDEO : https://drive.google.com/file/d/1JWc9BkXKekI_qARnIwiSsoFCEhx9kowu/view?usp=drive_link




