#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <ctime>
#include <cstdlib>
#include <sstream>

using namespace std;

// Hàm băm để lưu mật khẩu an toàn
string simpleHash(const string& password) {
    unsigned long hash = 5381;
    for (char c : password) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return to_string(hash);
}
// Hàm sinh OTP
string generateOTP(int length = 6) {
    const string digits = "0123456789";
    string otp = "";
    srand(time(0));
    for (int i = 0; i < length; ++i) {
        otp += digits[rand() % digits.length()];
    }
    return otp;
}
// Hàm khởi tạo mật khẩu ngẫu nhiên
string generateRandomPassword(int length = 8) {
    const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    string password = "";
    srand(time(0));
    for (int i = 0; i < length; ++i) {
        password += chars[rand() % chars.length()];
    }
    return password;
}

// Cấu trúc của User Account để lưu thông tin người dùng
struct User {
    string username;
    string passwordHash;
    int walletBalance;
    bool isTempPassword;
};

unordered_map<string, User> userDatabase;
unordered_map<string, string> adminDatabase;
// Lưu thông tin người dùng vào file cá nhân
void saveUserToFile(const User& user) {
    ofstream file(user.username + ".txt");
    if (file) {
        file << "Username: " << user.username << endl;
        file << "Password Hash: " << user.passwordHash << endl;
        file << "Is Temporary Password: " << user.isTempPassword << endl;
        file.close();
    } else {
        cout << "Error saving user data for " << user.username << "!" << endl;
    }
}


// Lưu toàn bộ người dùng vào file "users.txt"
void saveUsersToFile() {
    ofstream file("users.txt");
    for (const auto& pair : userDatabase) {
        file << pair.second.username << " " << pair.second.passwordHash << " " << pair.second.walletBalance << " " << pair.second.isTempPassword << "\n";
    }
    file.close();
}

// Lưu toàn bộ admin vào file "admins.txt"
void saveAdminsToFile() {
    ofstream file("admins.txt");
    for (const auto& pair : adminDatabase) {
        file << pair.first << " " << pair.second << "\n";
    }
    file.close();
}

// Tải dữ liệu người dùng từ file "users.txt" vào bộ nhớ
void loadUsersFromFile() {
    ifstream file("users.txt");
    if (!file) return;
    User user;
    while (file >> user.username >> user.passwordHash >> user.walletBalance >> user.isTempPassword) {
        userDatabase[user.username] = user;
    }
    file.close();
}

// Tải dữ liệu admin từ file "admins.txt" vào bộ nhớ
void loadAdminsFromFile() {
    ifstream file("admins.txt");
    if (!file) return;
    string username, passwordHash;
    while (file >> username >> passwordHash) {
        adminDatabase[username] = passwordHash;
    }
    file.close();
}

// Hàm tạo tài khoản người dùng mới
void createAccount() {
    string username, password;
    char choice;
    cout << "Enter username: ";
    cin >> username;
    if (userDatabase.find(username) != userDatabase.end()) {
        cout << "User already exists!" << endl;
        return;
    }
    cout << "Do you want to enter a password? (y/n): ";
    cin >> choice;
    bool isTemp = false;
    if (choice == 'y' || choice == 'Y') {
        do {
        cout << "Enter password (at least 8 characters): ";
        cin >> password;
        if (password.length() <= 7) {
            cout << "Password too short! Please enter at least 8 characters.\n";
        }
    } while (password.length() <= 7);

    } else {
        password = generateRandomPassword();
        isTemp = true;
        cout << "Generated password: " << password << " (The password will be official after OTP cornfirmation is done and please change it after your first login)" << endl;
    }
    
    string otp = generateOTP();
        cout << "Generated OTP: " << otp << " (Please enter OTP to confirm register User)" << endl;
        string enteredOtp;
        cin >> enteredOtp;
        if (enteredOtp != otp) {
            cout << "Invalid OTP! register canceled." << endl;
            
        }
    else{
        User newUser = {username, simpleHash(password), 100, isTemp};
        userDatabase[username] = newUser;
        saveUsersToFile(); // Lưu vào danh sách tổng hợp
        saveUserToFile(newUser); // Lưu vào file cá nhân
        cout << "Account created successfully!" << endl;
        }
    
    
}

// Hàm tạo tài khoản Admin
void createAdminAccount() {
    string username, password;
    cout << "Enter admin username: ";
    cin >> username;
    if (adminDatabase.find(username) != adminDatabase.end()) {
        cout << "Admin already exists!" << endl;
        return;
    }
    do {
        cout << "Enter password (at least 8 characters): ";
        cin >> password;
        if (password.length() <= 7) {
            cout << "Password too short! Please enter at least 8 characters.\n";
        }
    } while (password.length() <= 7);
    adminDatabase[username] = simpleHash(password);
    string otp = generateOTP();
        cout << "Generated OTP: " << otp << " (Please enter OTP to confirm register Admin)" << endl;
        string enteredOtp;
        cin >> enteredOtp;
        if (enteredOtp != otp) {
            cout << "Invalid OTP! register canceled." << endl;
            
        }
    else{
        saveAdminsToFile();
        cout << "Admin account created successfully!" << endl;
        }
    }
    
// Cho phép người dùng thay đổi tên đăng nhập hoặc mật khẩu
void changeInformation(const string& username){
    int choice;
    do{
        cout<< "---------------------------------------";
        cout<< "\nWhat information do you want to change?";
        cout<< "\n1. User name";
        cout<< "\n2. password";
        cout<< "\n3. Done";
        cout<< "\n---------------------------------------";
        cout<< "\nOption: ";
        cin >> choice;
        if(choice == 1){
            string newUsername;
            cout << "Please enter your new user name: ";
            cin >> newUsername;
            if (userDatabase.find(newUsername) != userDatabase.end()) {
                cout<< "User name already exists!";
                }
            else{
                userDatabase[username].username = newUsername;
                string otp = generateOTP();
                cout << "Generated OTP: " << otp << " (Please enter OTP to change information)" << endl;
                string enteredOtp;
                cin >> enteredOtp;
                if (enteredOtp != otp) {
                cout << "Invalid OTP! information changing canceled" << endl;
                }
                else{
                    saveUsersToFile();
                    cout << "User name change successful! New User name: " << newUsername << endl;
                }
            }
            
        }
        else if (choice == 2){
            string newPassword;
            do {
            cout << "Enter password (at least 8 characters): ";
            cin >> newPassword;
            if (newPassword.length() <= 7) {
                cout << "Password too short! Please enter at least 8 characters.\n";
            }
        } while (newPassword.length() <= 7);
            userDatabase[username].passwordHash = simpleHash(newPassword);
            userDatabase[username].isTempPassword = false;
            string otp = generateOTP();
            cout << "Generated OTP: " << otp << " (Please enter OTP to change information)" << endl;
            string enteredOtp;
            cin >> enteredOtp;
            if (enteredOtp != otp) {
                cout << "Invalid OTP! information changing canceled" << endl;
                }
            else{
            saveUsersToFile();
            cout << "Password reset successful! New password: " << newPassword << endl;
            }
        }
        else if (choice != 3){
           cout << "Invalid choice, try again." << endl; 
        }
    }while (choice != 3);

}
// Ghi lại nhật ký giao dịch vào file cá nhân của người dùng
void logTransaction(const string& username, const string& message) {
    ofstream file(username + ".txt", ios::app); // Mở file ở chế độ ghi tiếp (append)
    if (file) {
        time_t now = time(0);
        char* dt = ctime(&now); // Lấy thời gian hiện tại
        file << "[" << dt << "] " << message << endl;
        file.close();
    } else {
        cout << "Error logging transaction for " << username << "!" << endl;
    }
}
// Admin cộng hoặc trừ điểm trong ví người dùng
void adjustUserBalance() {
    string username;
    int amount;
    char operation;
    
    cout << "Enter username: ";
    cin >> username;
    
    if (userDatabase.find(username) == userDatabase.end()) {
        cout << "User not found!" << endl;
        return;
    }

    cout << "Enter '+' to add points or '-' to deduct points: ";
    cin >> operation;
    
    cout << "Enter amount: ";
    cin >> amount;

    if (amount <= 0) {
        cout << "Invalid amount! Please enter a positive number." << endl;
        return;
    }

    string otp = generateOTP();
    cout << "Generated OTP: " << otp << " (Please enter OTP to confirm adjustment)" << endl;
    string enteredOtp;
    cin >> enteredOtp;
    
    if (enteredOtp != otp) {
        cout << "Invalid OTP! Operation canceled." << endl;
        return;
    }

    if (operation == '+') {
        userDatabase[username].walletBalance += amount;
        logTransaction(username, "Admin added " + to_string(amount) + " points.");
        cout << "Successfully added " << amount << " points to " << username << "'s account." << endl;
    } 
    else if (operation == '-') {
        if (userDatabase[username].walletBalance < amount) {
            cout << "User does not have enough points!" << endl;
            return;
        }
        userDatabase[username].walletBalance -= amount;
        logTransaction(username, "Admin deducted " + to_string(amount) + " points.");
        cout << "Successfully deducted " << amount << " points from " << username << "'s account." << endl;
    } 
    else {
        cout << "Invalid operation! Please enter '+' or '-'." << endl;
        return;
    }

    saveUsersToFile();
}

// Xem lịch sử giao dịch của người dùng
void viewTransactionHistory(const string& username) {
    string filename = username + ".txt";
    ifstream file(filename);
    
    if (!file) {
        cout << "No transaction history found for " << username << "!" << endl;
        return;
    }
    
    cout << "\n--- Transaction History for " << username << " ---\n";
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
    cout << "--------------------------------------\n";
    
    file.close();
}


// Menu thao tác sau khi đăng nhập người dùng
void transactionMenu(const string& username) {
    int choice;
    do {
        cout << "---------------------------------------";
        cout << "\nMenu for " << username << ":";
        cout << "\n1. Check Wallet Balance";
        cout << "\n2. Transfer Points";
        cout << "\n3. Change account information";
        cout << "\n4. View Transaction History";
        cout << "\n5. Logout";
        cout << "\n---------------------------------------";
        cout << "\nChoose an option: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Your wallet balance: " << userDatabase[username].walletBalance << " points" << endl;
        } else if (choice == 2) {
            string recipient;
            int amount;
            cout << "Enter recipient username: ";
            cin >> recipient;
            if (userDatabase.find(recipient) == userDatabase.end()) {
                cout << "Recipient not found!" << endl;
                continue;
            }
            cout << "Enter amount to transfer: ";
            cin >> amount;
            if (userDatabase[username].walletBalance < amount) {
                cout << "Insufficient balance!" << endl;
            } else {
                string otp = generateOTP();
                cout << "Generated OTP: " << otp << " (Please enter to confirm transaction)" << endl;
                string enteredOtp;
                cin >> enteredOtp;
                if (enteredOtp != otp) {
                    cout << "Invalid OTP! Transaction canceled." << endl;
                }
                else{
                    userDatabase[username].walletBalance -= amount;
                    userDatabase[recipient].walletBalance += amount;
                    
                    logTransaction(username, "Sent " + to_string(amount) + " points to " + recipient);
                    logTransaction(recipient, "Received " + to_string(amount) + " points from " + username);

                    cout << "Transaction successful! " << amount << " points transferred to " << recipient << "." << endl;
                }
            }
        } else if (choice == 3){
            changeInformation(username);
        }
        else if (choice == 4) {
            viewTransactionHistory(username);
        }
        else if (choice != 5) {
            cout << "Invalid choice, try again." << endl;
        }
    } while (choice != 5);
}

// Xử lý đăng nhập người dùng
bool loginUser() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (userDatabase.find(username) == userDatabase.end()) {
        cout << "User not found!" << endl;
        return false;
    }
    if (userDatabase[username].passwordHash == simpleHash(password)) {
        cout << "Login successful!" << endl;
        if (userDatabase[username].isTempPassword) {
            cout << "Please change your temporary password immediately." << endl;
        }
        transactionMenu(username);
        return true;
    } else {
        cout << "Incorrect password!" << endl;
        return false;
    }
}
// Xem các bản ghi về người dùng
void viewUsers() {
    cout << "\nList of Users:" << endl;
    for (const auto& pair : userDatabase) {
        cout << "Username: " << pair.second.username << ", Balance: " << pair.second.walletBalance << " points" << endl;
    }
}

// Lựa chọn thay đổi mật khẩu cho người dùng
void resetPassword() {
    string username;
    cout << "Enter username to change information: ";
    cin >> username;
    if (userDatabase.find(username) == userDatabase.end()) {
        cout << "User not found!" << endl;
        return;
    }
    changeInformation(username);
}

// Admin menu
void adminMenu() {
    int choice;
    do {
        cout << "---------------------------------------";
        cout << "\nAdmin Menu:";
        cout << "\n1. View Users";
        cout << "\n2. Reset User Password";
        cout << "\n3. Adjust User Balance"; 
        cout << "\n4. Logout";
        cout << "\n---------------------------------------";
        cout << "\nChoose an option: ";
        cin >> choice;
        switch (choice) {
            case 1:
                viewUsers();
                break;
            case 2:
                resetPassword();
                break;
            case 3:
                adjustUserBalance(); 
                break;
            case 4:
                return;
            default:
                cout << "Invalid choice, try again." << endl;
        }
    } while (true);
}

// Đăng nhập cho Admin
bool loginAdmin() {
    string username, password;
    cout << "Enter admin username: ";
    cin >> username;
    cout << "Enter admin password: ";
    cin >> password;
    if (adminDatabase.find(username) != adminDatabase.end() && adminDatabase[username] == simpleHash(password)) {
        cout << "Admin login successful!" << endl;
        adminMenu();
        return true;
    } else {
        cout << "Incorrect admin credentials!" << endl;
        return false;
    }
}

int main() {
    loadUsersFromFile();
    loadAdminsFromFile();
    int choice;
    while (true) {
        cout<< "---------------------------------------";
        cout << "\n1. Register User\n2. Register Admin\n3. Login as User\n4. Login as Admin\n5. Exit\nChoose an option: ";
        cin >> choice;
        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                createAdminAccount();
                break;
            case 3:
                loginUser();
                break;
            case 4:
                loginAdmin();
                break;
            case 5:
                cout << "Exiting..." << endl;
                return 0;
            default:
                cout << "Invalid choice, try again." << endl;
        }
    }
}
