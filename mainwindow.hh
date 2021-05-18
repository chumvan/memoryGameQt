#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include "player.hh"
#include "card.hh"
#include <vector>
using Game_row_type = std::vector<Card>;
using Game_board_type = std::vector<std::vector<Card>>;
using Coordinate_type = std::pair<unsigned int,unsigned int>; // coordinate(x,y)


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void ask_init_values();
    void create_all_card();
    void announce_winner(const std::vector<Player>& players);
    // Testing
    void draw_button();
    void print_all_card_values();

private slots:
    void card_click_handle();
    void confirmButton_click_handle();
    void give_up_game();

private:
    unsigned int IMG_WIDTH = 40;
    unsigned int IMG_HEIGHT = 80;
    unsigned int CARD_WIDTH = 60;
    unsigned int CARD_HEIGTH = 100;


    unsigned int factor1_ = 1;
    unsigned int factor2_ = 1;
    unsigned int seed_ = 0;
    std::vector<Player> player_list_;
    Game_board_type game_board_;

    Ui::MainWindow *ui;
    std::vector<QString> current_open_card_;
    unsigned int turn_ = 0;
    bool gave_up_ = false;

};
#endif // MAINWINDOW_HH
