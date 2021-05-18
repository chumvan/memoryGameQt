#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <iostream>
#include <string>
#include <random>
#include <QPushButton>
#include <QInputDialog>
#include <QDebug>
#include <QGridLayout>
#include <QMessageBox>

const QString INIT_GAME_TEXT = "Init parameter for game";
const std::string GAME_OVER = "Game over!";

// Calculates the factors of the product
// such that the factor as near to each other as possible.
void calculate_factors(unsigned int product,
                       unsigned int& smaller_factor, unsigned int& bigger_factor)
{
    for(unsigned int i = 1; i * i <= product; ++i)
    {
        if(product % i == 0)
        {
            smaller_factor = i;
        }
    }
    bigger_factor = product / smaller_factor;
}

unsigned int next_free(Game_board_type& g_board, unsigned int start)
{
    // Finding out the number of rows and columns of the game board
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    // Starting from the given value
    for(unsigned int i = start; i < rows * columns; ++i)
    {
        if(g_board.at(i / columns).at(i % columns).get_visibility() == EMPTY)
        {
            return i;
        }
    }

    // Continuing from the beginning
    for(unsigned int i = 0; i < start; ++i)
    {
        if(g_board.at(i / columns).at(i % columns).get_visibility() == EMPTY)
        {
            return i;
        }
    }
    // You should never reach this
    std::cout << "No more empty spaces" << std::endl;
    return rows * columns - 1;
}

// Fills the game board, the size of which is rows * columns, with empty cards.
void init_with_empties(Game_board_type& g_board, unsigned int rows, unsigned int columns)
{
    g_board.clear();
    Game_row_type row;
    for(unsigned int i = 0; i < columns; ++i)
    {
        Card card;
        row.push_back(card);
    }
    for(unsigned int i = 0; i < rows; ++i)
    {
        g_board.push_back(row);
    }
}

// Initializes the given game board (g_board) with randomly generated cards,
// based on the given seed value.
void init_with_cards(Game_board_type& g_board, int seed)
{

    // Finding out the number of rows and columns of the game board
    unsigned int rows = g_board.size();
    unsigned int columns = g_board.at(0).size();

    // Drawing a cell to be filled
    std::default_random_engine randomEng(seed);
    std::uniform_int_distribution<int> distr(0, rows * columns - 1);

    // Wiping out the first random number (that is always the lower bound of the distribution)
    distr(randomEng);


    // If the drawn cell is already filled with a card, next empty cell will be used.
    // (The next empty cell is searched for circularly, see function next_free.)
    for(unsigned int i = 0, c = 'A'; i < rows * columns - 1; i += 2, ++c)
    {
        // Adding two identical cards (pairs) in the game board
        for(unsigned int j = 0; j < 2; ++j)
        {
            unsigned int cell = distr(randomEng);
            cell = next_free(g_board, cell);
            g_board.at(cell / columns).at(cell % columns).set_letter(c);
            g_board.at(cell / columns).at(cell % columns).set_visibility(HIDDEN);
        }
    }
}

// Check if the game finished by playing till the end
bool is_won(const Game_board_type& game_board){
    for(const auto& row : game_board){
        for(const auto& cell : row){
            if(cell.get_visibility() != EMPTY) return false;
        }
    }
    return true;
}

// Dislplay the winner in a message then exit the program
void MainWindow::announce_winner(const std::vector<Player>& players){

    std::vector<Player> winner_list;
    unsigned int max_pairs = players[0].number_of_pairs();

    for(size_t indx = 0; indx < players.size();indx++)
    {
        if(players[indx].number_of_pairs() > max_pairs)
        {
            winner_list.clear();
            winner_list.push_back(players[indx]);
            max_pairs = players[indx].number_of_pairs();
        }
        else if(players[indx].number_of_pairs() == max_pairs){
            winner_list.push_back(players[indx]);
        }
    }

    QString winnerInfo = "";
    if(winner_list.size() > 1)
    {
        winnerInfo = QString{"Tie of %1 players with %2"}.
                arg(winner_list.size()).
                arg(winner_list[0].number_of_pairs());
        return;
    }
    if(winner_list.size() == 1)
    {
        winnerInfo = QString{"%1 has won with %2 pairs"}.
                arg(QString::fromStdString(winner_list[0].get_name())).
                arg(winner_list[0].number_of_pairs());
    }

    QMessageBox::information(this,tr("Game Over"),winnerInfo);
    MainWindow::close();
}

// Card clicked -> set visibility and change the displayed image
void MainWindow::card_click_handle()
{
    QPushButton *clicked_card = qobject_cast<QPushButton *>(sender());
    QStringList parse_object_name = clicked_card->objectName().split(QString('_'));
    if(current_open_card_.size() < 2)
    {
        game_board_.at(parse_object_name[1].toInt()).
                at(parse_object_name[2].toInt()).
                set_visibility(OPEN);

        QPixmap image(":/"+parse_object_name[0]+".png");
        image = image.scaled(IMG_WIDTH, IMG_HEIGHT);
        clicked_card->setIcon(image);
        clicked_card->setIconSize(image.rect().size());
        current_open_card_.push_back(clicked_card->objectName());
        }



}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->confirmButton,SIGNAL(clicked()),this,SLOT(confirmButton_click_handle()));
    connect(ui->giveupButton,SIGNAL(clicked()),this,SLOT(give_up_game()));

    ask_init_values();
    init_with_empties(game_board_,factor1_, factor2_);
    init_with_cards(game_board_, seed_);
    print_all_card_values();
    create_all_card();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Enter players with name and number of cards
void MainWindow::ask_init_values()
{
    unsigned int player_num = QInputDialog::getInt(this,INIT_GAME_TEXT,
                                         "Number of players | min = 2 | max = 4 ",2,2,4,1);
    for(unsigned int i = 0; i < player_num; i++)
    {
        player_list_.push_back(QInputDialog::getText(this,
                                                     INIT_GAME_TEXT,
                                                     "Player's name").toStdString());
    }
    unsigned int card_num = QInputDialog::getInt(this,INIT_GAME_TEXT,
                                         "Number of cards | min = 10 | max = 20 ",10,10,20,2);
    seed_ = QInputDialog::getInt(this,INIT_GAME_TEXT,
                                         "Random seed");
    calculate_factors(card_num,factor1_,factor2_);

}

// Help function to see value of cards
void MainWindow::print_all_card_values()
{
    unsigned int rows = game_board_.size();
    unsigned int columns = game_board_.at(0).size();
    for(unsigned int r = 0; r < rows; r++)
    {
        for(unsigned c = 0; c < columns; c++)
        {
            game_board_.at(r).at(c).turn();
            game_board_.at(r).at(c).print();
        }
        std::cout << std::endl;
    }
}

// Initialize cards with their state of visibility
void MainWindow::create_all_card()
{   
    QPushButton *cards[factor1_][factor2_];
    for(unsigned int r = 0; r < factor1_; r++)
    {
        for(unsigned int c = 0; c < factor2_; c++)
        {

            QChar buttonText = QChar::fromLatin1(game_board_.at(r).at(c).get_letter());
            cards[r][c] = new QPushButton();
            QString temp_object_name = QString(buttonText) + QString{"_%1_%2"}.arg(r).arg(c);
            cards[r][c]->setObjectName(temp_object_name);
            cards[r][c]->setFixedSize(QSize(CARD_WIDTH,CARD_HEIGTH));
            QPixmap image(QString::fromStdString(":/cardBack.png"));
            image = image.scaled(IMG_WIDTH, IMG_HEIGHT);
            cards[r][c]->setIcon(image);
            cards[r][c]->setIconSize(image.rect().size());
            ui->gridLayout->addWidget(cards[r][c],r,c);
            cards[r][c]->show();
            connect(cards[r][c],SIGNAL(clicked()),this,SLOT(card_click_handle()));
        }
    }
    ui->labelPlayerName->setText(QString::fromStdString(player_list_[turn_%player_list_.size()].get_name()));
}

// After user pick 2 cards, handle the logic if that was a right pair or not
// Check if the game is finished
void MainWindow::confirmButton_click_handle()
{
    if(current_open_card_.size() == 2)
    {
        QPushButton *button1 = ui->gridLayoutWidget->findChild<QPushButton *>(current_open_card_[0]);
        QPushButton *button2 = ui->gridLayoutWidget->findChild<QPushButton *>(current_open_card_[1]);
        QString value1 = button1->objectName().split(QString('_'))[0];
        unsigned int x1 = button1->objectName().split(QString('_'))[2].toInt();
        unsigned int y1 = button1->objectName().split(QString('_'))[1].toInt();

        QString value2 = button2->objectName().split(QString('_'))[0];
        unsigned int x2 = button2->objectName().split(QString('_'))[2].toInt();
        unsigned int y2 = button2->objectName().split(QString('_'))[1].toInt();

        game_board_.at(y1).at(x1).set_visibility(OPEN);
        game_board_.at(y2).at(x2).set_visibility(OPEN);
        if(value1 == value2)
        {
            // Handle logic
            // Add correct card to player
            player_list_[turn_%player_list_.size()].add_card(
                        game_board_.at(y1).at(x1));

            // Handle display
            // Cards display
            ui->labelInfo->setText("Right pair");
            QPixmap image(QString::fromStdString(":/checked.png"));
            image = image.scaled(IMG_WIDTH, IMG_HEIGHT);

            button1->setDisabled(true);
            button1->setIcon(image);
            button1->setIconSize(image.rect().size());
            game_board_.at(y1).at(x1).set_visibility(EMPTY);

            button2->setDisabled(true);
            button2->setIcon(image);
            button2->setIconSize(image.rect().size());
            game_board_.at(y2).at(x2).set_visibility(EMPTY);


        }
        else{
            ui->labelInfo->setText("Not a pair - Next player !");
            QPixmap image(QString::fromStdString(":/cardBack.png"));
            image = image.scaled(IMG_WIDTH, IMG_HEIGHT);

            // Handle display
            button1->setIcon(image);
            button1->setIconSize(image.rect().size());

            button2->setIcon(image);
            button2->setIconSize(image.rect().size());

            game_board_.at(y1).at(x1).set_visibility(HIDDEN);
            game_board_.at(y2).at(x2).set_visibility(HIDDEN);

            turn_++;

        }
        ui->labelPlayerName->setText(
                    QString::fromStdString(
                                         player_list_[turn_%player_list_.size()].get_name()));
        // Correct pairs display
        ui->labelCorrectPairs->clear();
        ui->labelCorrectPairs->setText(
                    QString::fromStdString(player_list_[turn_%player_list_.size()]
                                       .get_correct_pairs()));
        // Points gained display
        ui->lcdNumber->display(int(player_list_[turn_%player_list_.size()].number_of_pairs()));
        if(is_won(game_board_) || gave_up_)
        {
            ui->labelInfo->setText("Game over!");
            announce_winner(player_list_);
        }
        current_open_card_.clear();
    }

}

// Exit the game earlier and show winner
void MainWindow::give_up_game()
{
    gave_up_ = true;
    ui->labelInfo->setText("Game over!");
    announce_winner(player_list_);
}





