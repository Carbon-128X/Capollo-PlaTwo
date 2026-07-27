#include "usermanager.h"
#include "filemanager.h"
#include <QMessageBox>
#include "custommessagebox.h"
QVector<User> UserManager::users;
User UserManager::currentUser;
QVector<GameHistory> UserManager::history;
void UserManager::load() {
    users = FileManager::loadUsers();
    history=FileManager::loadHistory();
}

void UserManager::save() {
    FileManager::saveUsers(users);
    FileManager::saveHistory(history);
}

bool UserManager::usernameExists(QString username) {
    for(const User &user : users) {
        if(user.username == username){
            return true;
        }
    }

 return false;
}

bool UserManager::phoneExists(QString phone) {
    for(const User &user : users) {
        if(user.phone == phone){
            return true;
        }
    }

    return false;
}

bool UserManager::emailExists(QString email) {
    for(const User &user : users) {
        if(user.email == email){
            return true;
        }
    }

 return false;
}

// این قسمتو واسه ادیت پروفایل اضافع کردم

bool UserManager::usernameExistsExceptCurrent(QString username) {
    for(const User &user : users) {
        if(user.username == username && user.username != currentUser.username){
            return true;
        }
    }

 return false;
}

bool UserManager::phoneExistsExceptCurrent(QString phone) {
    for(const User &user : users) {
        if(user.phone == phone && user.username != currentUser.username){
            return true;
        }
    }

 return false;
}

bool UserManager::emailExistsExceptCurrent(QString email){
    for(const User &user : users) {
        if(user.email == email && user.username != currentUser.username){
            return true;
        }
    }

 return false;
}

bool UserManager::registerUser(const User &user) {
    load();
    if(usernameExists(user.username)){
        return false;
    }
    if(phoneExists(user.phone)){
        return false;
    }
    if(emailExists(user.email)){
        return false;
    }
        users.push_back(user);
        save();
        return true;
}

bool UserManager::login(QString username, QString passwordHash) {
    load();
   for(const User &user : users) {
        if(user.username == username && user.passwordHash == passwordHash){
            currentUser = user;
            return true;
        }
    }
 return false;
}

bool UserManager::changePassword(QString phone, QString newPasswordHash) {
    load();
    for(User &user : users) {
        if(user.phone == phone) {
            user.passwordHash = newPasswordHash;
            save();
            return true;
        }
    }

 return false;
}

User* UserManager::findUser(QString username) {
    for(User &user : users) {
        if(user.username == username){
            return &user;
        }
    }

 return nullptr;
}

bool UserManager::updateUser(const User &newUser) {
    load();
     for(User &user : users){
        if(user.username == currentUser.username){
            user = newUser;
            currentUser = newUser;
            save();
            return true;
        }
    }

 return false;
}

void UserManager::addHistory(const GameHistory &game) {
    load();

    history.push_back(game);
    FileManager::saveHistory(history);
}

QVector<GameHistory> UserManager::getHistory(QString username, QString game) {
    load();
    QVector<GameHistory> result;
    for(const GameHistory &item : history) {
        if(item.username==username && item.game==game) {
            result.push_back(item);
        }
    }

 return result;
}
