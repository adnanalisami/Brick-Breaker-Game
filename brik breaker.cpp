
//kisi bhi project ko start karne ke liye sab se ziada zaroori hoti hein us ki help ke liye special libraries 


#include <iostream>//ye input output ke liye hota hai 
#include <conio.h>//ye getch ya phir kb hit wagera ke liye hota hai ,,wo alag baat hai ke visual studio main getch or kb hit properly 
//handle nmahi hota is liye hamein _ ka symbol use karna parta hai do no functions ko use karne se pahle ,lekin phir bi ye library zaroori hoti hai 
#include <windows.h>//sleep ya phir cls ke liye use hoti hai
#include <cstring>//.ye to zahir hai ke string ke liye hia ,,,mere khayaaal se ye string ke functions ko iuse karne ke liye include
//ki jaati hai ,,,i think
#include <string>//ye simple string ko use karne ke liye
#include <fstream>//hamaare code main waise ziada file handling ki zaroorat to nahi hai magar as per requirement kuchh na kuchh 
//hum file handling use kareinge jaise player ka naam wagera oor score or agar wo toperr hai to us kok bhi hum congress kareineg

using namespace std;//cin ,cout

//ab uaha se hammara game ki proper logic design kareinge ,,,
// kisi bhi game ke liye sab se ziada zaroori hota hai us ki boundary lines ,,kionke hum ne kisi bhi game ko us ki boundary main hi challana hai,,
//us ke liye hum screen ki height or width oor boundary ko initialize kareinge
#define screen_height 30
#define screen_width 60
#define min_width_bound 1
#define max_width_bound (screen_width - 2)
#define min_height_bound 3        // Changed from 1 to 3 to protect top border at y=2
#define max_height_bound (screen_height - 2)

int score = 0;//score ke baghari to sir system yumpossible hei

void goto_spec(int x, int y) {
    COORD coord;
    coord.X = x;     //ye function hum ne is liye bnaya hai ke hum ne kuchh khaads axis pai ja kar hi kuchh likhna ya bnana ho ga ,
    //border wagera ,,
    //simply ye x axis oor y axis ko le ga us axis pai ja kar text ko likh dega 
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);//ab sawaal ye paida hota hai ke ye kiya hai ??
    //to sir ye hum consoile ke zariey kareing handle aik data type hai oor coord bhindata ty[pe hai jis se hummcoordinates ka andaaza lagate hain
}

void set_text_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);//ye bhinsame hai lekin colour ke liye hai 
}

void reset_color() {
    set_text_color(7);//is function ki khaas zaroorat to nahi hai lekin user friendly ke liye behtr hai ye main setcolour ko use karke bnaya hai 
    //ke jab userr ko defoult colour maoin previwe required ho to wo reset colour se gray colour ko call karege 
}

int paddle_pos[1][2] = { {screen_height - 2, screen_width / 2 - 4} };//ab ye hai paddle position ke liye ..
//ye baat hamesha yaad rakhi jaye ke har baar jab bhiu hum position ki baat kareinge to row oor columns oor x or y axis ki baat kareineg
int ball_pos[1][2] = { {screen_height - 3, screen_width / 2} };//is ka bhi same sceen hai ,,lekin agar aap is ki location dekhien to 
//baal ke bilkul ye paddmle aye ga
int ball_dx = 1//is main x axis ki jaanib move kareineg
, ball_dy = -1;//oor is main hum y axis ki jaanab move karsakein ge

int bricks_pos[30][2];//ab yahan par aik ghoor talb baat hai wo ye ke ye array briks ki position ko store karrahi hai na k briks ko ..
bool brick_visible[30] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };//ab yahan visibilty ki traf chalte hein ke ball visible hein ke nauhi to 3o briks hain 

void initialize_bricks() {
    int brick_count = 0;
    for (int row = 3; row <= 6; row++) {//ye loop rows ko handle karegs ke mujhe kitni rows chaiye briljs ki
        for (int col = 6; col <= screen_width - 6; col += 7) {//oor ye columns ko handelkarega
            bricks_pos[brick_count][0] = row;//oor ye row oor col ki poition hijaye gy
            bricks_pos[brick_count][1] = col;//ab ye sab se zaroori cheex hai ke aya ke aik brik bnegy jahan per 
            brick_visible[brick_count] = true;
            brick_count++;
            if (brick_count >= 30)
                break;//.to ensure that 30 briks are coimpltet oor not
        }
    }
}

void draw_border() {
    set_text_color(4);
    for (int i = 0; i < screen_width; i++) {
        goto_spec(i, 2); cout << "=";  // Top border at y=2
        goto_spec(i, screen_height); cout << "=";
    }
    for (int i = 2; i <= screen_height; i++) {
        goto_spec(0, i); cout << "|";//ye to simple drawinga hein ,,border draw karne ke liye 
        goto_spec(screen_width - 1, i); cout << "|";
    }
    reset_color();
    goto_spec(2, 0); cout << "=== BRICK BREAKER ===";
    goto_spec(2, 1); cout << "Score: " << score << "    ";
}

void erase_elements() {
    // Erase ball
    goto_spec(ball_pos[0][1], ball_pos[0][0]);
    cout << " ";

    // Erase paddle
    for (int i = 0; i < 8; i++) {//yahan 8 see muraad hargiz 8 paddle nahi hein is se muraad is ke 8 chars hein 
        goto_spec(paddle_pos[0][1] + i, paddle_pos[0][0]);
        cout << " ";
    }
}

void draw_elements() {
    // Draw paddle
    goto_spec(paddle_pos[0][1], paddle_pos[0][0]);
    set_text_color(2); cout << "========"; reset_color();

    // Draw ball
    if (ball_pos[0][0] >= min_height_bound && ball_pos[0][0] <= max_height_bound &&
        ball_pos[0][1] >= min_width_bound && ball_pos[0][1] <= max_width_bound) {
        goto_spec(ball_pos[0][1], ball_pos[0][0]);
        set_text_color(3); cout << "O"; reset_color();
    }
}

void draw_bricks() {
    set_text_color(1);
    for (int i = 0; i < 30; i++) {
        if (brick_visible[i]) {//ab yahan per humnne briks ko draw kiya hai specific location per 
            goto_spec(bricks_pos[i][1], bricks_pos[i][0]);
            cout << "###";
        }
    }
    reset_color();
}

bool is_game_over() {
    return (ball_pos[0][0] >= max_height_bound);//ab ye bhi aik normal cheez hai game over ka logic
}

bool is_game_won() {
    for (int i = 0; i < 30; i++) {
        if (brick_visible[i]) return false;//saari briks agar toot gaye hein to game win ha 
    }
    return true;
}

void show_instructions() {
    system("cls");
    goto_spec(10, 5); cout << "INSTRUCTIONS:";
    goto_spec(10, 7); cout << "- Press A 0r left arrow key to move left";
    goto_spec(10, 8); cout << "- Press D or right arrow to move right";
    goto_spec(10, 9); cout << "- Avoid missing the ball!";
    goto_spec(10, 11); cout << "Press any key to go back...";
    _getch();
}

void hide_cursor() {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false; // hide cursor
    SetConsoleCursorInfo(out, &cursorInfo);
}

void start_game(string name) {
    // Reset game state properly
    score = 0;
    ball_pos[0][0] = screen_height - 3;
    ball_pos[0][1] = screen_width / 2;
    paddle_pos[0][1] = screen_width / 2 - 4;
    initialize_bricks();

    draw_border();
    bool game_running = true;

    while (game_running) {
        erase_elements();

        // Ball movement logic
        ball_pos[0][1] += ball_dx;
        ball_pos[0][0] += ball_dy;

        // Wall collision
        if (ball_pos[0][1] <= min_width_bound + 1 || ball_pos[0][1] >= max_width_bound - 1)
            ball_dx *= -1;
        if (ball_pos[0][0] < min_height_bound) {  // Fixed top wall collision
            ball_pos[0][0] = min_height_bound;
            ball_dy *= -1;
        }

        // Game over check
        if (ball_pos[0][0] >= max_height_bound) {
            game_running = false;
            continue;
        }

        // Paddle collision
        if (ball_pos[0][0] == paddle_pos[0][0] - 1 &&
            ball_pos[0][1] >= paddle_pos[0][1] && ball_pos[0][1] <= paddle_pos[0][1] + 7) {
            ball_dy *= -1;
        }

        // Brick collision
        for (int i = 0; i < 30; i++) {
            if (brick_visible[i] && ball_pos[0][0] == bricks_pos[i][0] &&
                ball_pos[0][1] >= bricks_pos[i][1] && ball_pos[0][1] <= bricks_pos[i][1] + 2) {
                brick_visible[i] = false;
                score += 100;
                goto_spec(9, 1); cout << score;
                ball_dy *= -1;
                break;
            }
        }

        // Player input
        if (_kbhit()) {
            char ch = _getch();
            if ((ch == 'a' || ch == 'A' || ch == 75) && paddle_pos[0][1] > min_width_bound)
                paddle_pos[0][1] -= 3;
            else if ((ch == 'd' || ch == 'D' || ch == 77) && (paddle_pos[0][1] + 8) < max_width_bound)
                paddle_pos[0][1] += 3;
            else if (ch == 27) // ESC key
                game_running = false;
        }

        draw_elements();
        draw_bricks();

        // End game checks
        if (is_game_over()) {
            goto_spec(screen_width / 2 - 5, screen_height / 2);
            set_text_color(1); cout << "GAME OVER"; reset_color();
            break;
        }

        if (is_game_won()) {
            goto_spec(screen_width / 2 - 5, screen_height / 2);
            set_text_color(2); cout << "YOU WIN!"; reset_color();
            break;
        }

        Sleep(30);
    }

    // Save score
    fstream myfile("record.txt", ios::app);
    if (myfile.is_open()) {
        myfile << name << "    " << score << endl;
    }

    // Clear game screen before returning to menu
    goto_spec(0, screen_height + 1);
    cout << "Press any key to return to menu...";
    _getch();
    system("cls"); // Clear game over message before showing menu
}

void show_menu(string name) {
    char choice;
    do {
        system("cls");
        goto_spec(20, 5); cout << "===== BRICK BREAKER MENU =====";
        goto_spec(20, 7); cout << "1. Start Game";
        goto_spec(20, 8); cout << "2. Instructions";
        goto_spec(20, 9); cout << "3. View Previous Records";
        goto_spec(20, 10); cout << "4. Exit";
        goto_spec(20, 12); cout << "Enter your choice: ";

        choice = _getch();

        switch (choice) {
        case '1':
            system("cls");
            start_game(name);
            break;
        case '2':
            show_instructions();
            break;
        case '3': {
            system("cls");
            fstream readfile("record.txt", ios::in);
            if (readfile.is_open()) {
                string line;
                while (getline(readfile, line)) {
                    cout << line << endl;
                }
                readfile.close();
            }
            _getch();
            break;
        }
        case '4':
            goto_spec(20, 13); cout << "Exiting..."; Sleep(1000);
            break;
        default:
            goto_spec(20, 13); cout << "Invalid choice! Try again."; Sleep(1000);
        }
    } while (choice != '4');
}

int main() {
    string name;
    char enroll_choice;
    cout << "Do you want to enroll yourself (Y/N)? ";
    cin >> enroll_choice;
    cin.ignore();

    if (enroll_choice == 'Y' || enroll_choice == 'y') {
        cout << "Please enter your name: ";
        getline(cin, name);

        fstream myfile("record.txt", ios::app);
        if (myfile.is_open()) {
            myfile << "\nPlayer Name    Score\n";
        }
    }
    else {
        name = "Guest";
    }

    hide_cursor();
    show_menu(name);
    return 0;
}