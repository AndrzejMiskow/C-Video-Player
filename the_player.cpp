//
// Created by twak on 11/11/2019.
//

#include "the_player.h"

using namespace std;

// all buttons have been setup, store pointers here
void ThePlayer::setContent(std::vector<TheButton*>* b, std::vector<TheButtonInfo>* i) {
    buttons = b;
    infos = i;
    jumpTo(buttons -> at(0) -> info);
}



void ThePlayer::playStateChanged (QMediaPlayer::State ms) {
    switch (ms) {
        case QMediaPlayer::State::StoppedState:
            pause(); // starting playing again...
            emit(stateChanged(this->state())); //emit a signal so the play/pause button knows
            break;
    default:
        break;
    }
}

void ThePlayer::jumpTo (TheButtonInfo* button) {
    setMedia( * button -> url);
    pause();
    emit(stateChanged(this->state()));
}

void ThePlayer::newContent(TheButtonInfo i){
    infos->push_back(i);
    jumpTo(&i);//set newest video to also be one playing
    emit(stateChanged(this->state()));
}
