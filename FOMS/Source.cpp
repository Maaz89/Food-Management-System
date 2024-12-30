#include <iostream>
#include <string>

using namespace std;

// Function to print a centered message
void printCentered(string message, int width = 120) {
    int padding = (width - message.length()) / 2;
    for (int i = 0; i < padding; ++i) cout << ' ';
    cout << message << endl;
}

// Node for a singly linked list
class ListNode {
public:
    string data;
    ListNode* next;

    ListNode(string value) : data(value), next(nullptr) {}
};

// Class representing a food item in the BST
class FoodNode {
public:
    int foodNo;
    string name;
    double price;
    int inStock;
    string category;
    int totalSold;
    FoodNode* left;
    FoodNode* right;

    // Constructor to initialize a node
    FoodNode(int number, string foodName, double foodPrice, int stock, string cat)
        : foodNo(number), name(foodName), price(foodPrice), inStock(stock),
        category(cat), totalSold(0), left(nullptr), right(nullptr) {}
};

// Class for managing the entire food management system
class FoodManagementSystem {
private:
    FoodNode* root; // Root of the food BST
    ListNode* orderHistory; // Linked list for order history
    ListNode* adminLogs; // Linked list for admin logs
    double totalRevenue; // Tracks total revenue

public:
    // Constructor
    FoodManagementSystem() : root(nullptr), orderHistory(nullptr), adminLogs(nullptr), totalRevenue(0) {}

    // Insert a new food item into the BST
    void insertFood(int number, string name, double price, int stock, string category) {
        root = insert(root, number, name, price, stock, category);
        addAdminLog("Added Food Item: " + name + " (" + category + ")");
    }

    // Recursive BST insertion logic
    FoodNode* insert(FoodNode* node, int number, string name, double price, int stock, string category) {
        if (node == nullptr) {
            return new FoodNode(number, name, price, stock, category);
        }
        if (number < node->foodNo) {
            node->left = insert(node->left, number, name, price, stock, category);
        }
        else if (number > node->foodNo) {
            node->right = insert(node->right, number, name, price, stock, category);
        }
        return node;
    }

    // Display the food list using in-order traversal
    void displayFoodList(FoodNode* node) {
        if (node == nullptr) return;
        displayFoodList(node->left);
        cout << "No: " << node->foodNo << " | Name: " << node->name
            << " | Price: $" << node->price << " | In Stock: " << node->inStock
            << " | Category: " << node->category
            << endl;
        displayFoodList(node->right);
    }

    // Entry point to show the available menu
    void showFoodList() {
        cout << "\nAvailable Food Items:\n";
        displayFoodList(root);
    }

    // Search for a food item by number
    FoodNode* findFood(int number) {
        return findFood(root, number);
    }

    // Recursive logic for searching in BST
    FoodNode* findFood(FoodNode* node, int number) {
        if (node == nullptr || node->foodNo == number) return node;
        if (number < node->foodNo) return findFood(node->left, number);
        return findFood(node->right, number);
    }

    // Add to order history linked list
    void addOrderHistory(string orderDetail) {
        ListNode* newOrder = new ListNode(orderDetail);
        newOrder->next = orderHistory;
        orderHistory = newOrder;
    }

    // Add to admin logs linked list
    void addAdminLog(string logDetail) {
        ListNode* newLog = new ListNode(logDetail);
        newLog->next = adminLogs;
        adminLogs = newLog;
    }

    // Handle customer food orders
    void handleCustomerOrder() {
        int orderNo, quantity;
        char continueOrder;
        double totalOrderPrice = 0; // Variable to store the total price of the order

        do {
            showFoodList();
            cout << "\nEnter the food number to order: ";
            cin >> orderNo;
            cout << "Enter the quantity: ";
            cin >> quantity;

            FoodNode* food = findFood(orderNo);
            if (food && food->inStock >= quantity) {
                double totalPrice = food->price * quantity;
                cout << "\nItem: " << food->name << " | Quantity: " << quantity << " | Price: $" << totalPrice << endl;

                // Deduct stock and update total sold
                food->inStock -= quantity;
                food->totalSold += quantity;
                totalRevenue += totalPrice;

                // Add to the total order price
                totalOrderPrice += totalPrice;

                // Record order history
                addOrderHistory("Ordered " + to_string(quantity) + " x " + food->name + " for $" + to_string(totalPrice));
            }
            else {
                cout << "\nInvalid choice or insufficient stock for item: " << orderNo << ".\n";
            }

            cout << "\nDo you want to order another item? (y/n): ";
            cin >> continueOrder;

        } while (tolower(continueOrder) == 'y');

        // Display the total order price after customer finishes ordering
        cout << "\nTotal Order Price: $" << totalOrderPrice << endl;

        handlePayment();
    }

    // Simulate payment handling logic
    void handlePayment() {
        int choice;
        cout << "\nPayment Options:\n";
        cout << "1. Cash Payment\n";
        cout << "2. Credit Card Payment\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "\nPayment successful. Your food has been ordered.\n";
        }
        else if (choice == 2) {
            string cardNumber, cardPassword;

            // Loop until a valid card number is entered
            while (true) {
                cout << "\nEnter your credit card number (16 digits): ";
                cin >> cardNumber;  // Taking card number input

                // Check if the card number is exactly 16 digits long
                if (cardNumber.length() == 16) {
                    break;  // Exit the loop if the card number is valid
                }
                else {
                    cout << "Error: Invalid card number. Please enter exactly 16 digits.\n";
                }
            }

            // Ask for card password
            cout << "Enter your card password: ";
            cin >> cardPassword;  // Taking card password input

            // Validate the card details
            if (validateCard(cardNumber, cardPassword)) {
                cout << "\nPayment successful. Your food has been ordered.\n";
            }
            else {
                cout << "\nInvalid card details. Returning to main menu.\n";
            }
        }
        else {
            cout << "\nInvalid choice. Returning to main menu.\n";
        }
    }

    // Simple validation function for card number and password
    bool validateCard(const string& cardNumber, const string& cardPassword) {
        if (!cardPassword.empty()) {
            return true;
        }
        return false;
    }

    // Simulate admin login
    bool adminLogin() {
        string username, password;
        const string ADMIN_USER = "admin";
        const string ADMIN_PASS = "password";

        cout << "Enter admin username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        if (username == ADMIN_USER && password == ADMIN_PASS) {
            cout << "\nLogin successful!\n";
            return true;
        }

        cout << "\nInvalid credentials.\n";
        return false;
    }

    // Admin menu for modifying data
    void adminMenu() {
        int choice;
        do {
            cout << "\nAdmin Panel\n";
            cout << "1. Add Food Item\n";
            cout << "2. View Food List\n";
            cout << "3. View Order History\n";
            cout << "4. View Sales Report\n";
            cout << "5. View Admin Logs\n";
            cout << "6. Exit to Main Menu\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1: {
                int no, stock;
                string name, category;
                double price;

                cout << "Enter Food No: ";
                cin >> no;
                cout << "Enter Food Name: ";
                cin >> ws;
                getline(cin, name);
                cout << "Enter Food Price: ";
                cin >> price;
                cout << "Enter In-Stock Quantity: ";
                cin >> stock;
                cout << "Enter Category: ";
                cin >> ws;
                getline(cin, category);

                insertFood(no, name, price, stock, category);
                cout << "\nFood item added successfully!\n";
                break;
            }
            case 2:
                showFoodList();
                break;
            case 3:
                viewOrderHistory();
                break;
            case 4:
                viewSalesReport();
                break;
            case 5:
                viewAdminLogs();
                break;
            case 6:
                cout << "\nReturning to Main Menu...\n";
                break;
            default:
                cout << "\nInvalid choice. Please try again.\n";
            }
        } while (choice != 6);
    }

    // View order history
    void viewOrderHistory() {
        cout << "\nOrder History:\n";
        ListNode* temp = orderHistory;
        while (temp) {
            cout << "- " << temp->data << endl;
            temp = temp->next;
        }
    }

    // View sales report
    void viewSalesReport() {
        cout << "\nSales Report:\n";
        cout << "Total Revenue: $" << totalRevenue << endl;
        cout << "Best-Selling Items:\n";
        displayTopSellingItems(root);
    }

    // Display best-selling items
    void displayTopSellingItems(FoodNode* node) {
        if (node == nullptr) return;
        displayTopSellingItems(node->left);
        if (node->totalSold > 0) {
            cout << "- " << node->name << " | Sold: " << node->totalSold << endl;
        }
        displayTopSellingItems(node->right);
    }

    // View admin activity logs
    void viewAdminLogs() {
        cout << "\nAdmin Activity Logs:\n";
        ListNode* temp = adminLogs;
        while (temp) {
            cout << "- " << temp->data << endl;
            temp = temp->next;
        }
    }

    // Main menu loop
    void mainMenu() {
        int choice;
        do {
            cout << "\nMain Menu\n";
            cout << "1. Food List & Order\n";
            cout << "2. Admin Panel\n";
            cout << "3. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                handleCustomerOrder();
                break;
            case 2:
                if (adminLogin()) {
                    adminMenu();
                }
                break;
            case 3:
                cout << "\nExiting system...\n";
                break;
            default:
                cout << "\nInvalid choice, please try again.\n";
            }
        } while (choice != 3);
    }
};

int main() {
    // Enhanced welcome message
    cout << endl;
    printCentered("=====================================");
    printCentered(" Welcome to the Food Management System ");
    printCentered("=====================================");
    cout << endl;
    printCentered("Enjoy delicious food and efficient service!");
    cout << endl;

    FoodManagementSystem fms;

    // Preload some food items
    fms.insertFood(51, "Burger", 5.99, 20, "Snacks");
    fms.insertFood(20, "Pizza", 8.99, 15, "Main Course");
    fms.insertFood(71, "Pasta", 6.49, 25, "Main Course");
    fms.insertFood(67, "Ice Cream", 3.99, 30, "Desserts");
    fms.insertFood(80, "Salad", 4.99, 10, "Appetizers");

    // Start the main menu
    fms.mainMenu();

    return 0;
}
