#include <bits/stdc++.h>
using namespace std;

// Class to store product details
class Product
{
public:
    int id;
    string name;
    double price;

    Product(int id, string name, double price) : id(id), name(name), price(price) {}
};

// Class to store user details
class User
{
public:
    int id;
    string name;

    User(int id, string name) : id(id), name(name) {}
};

class BillingSystem
{
private:
    vector<Product> products;
    vector<pair<Product, int>> cart;
    vector<User> users;
    int currentUserId;

public:
    BillingSystem() : currentUserId(-1) // Initialize with no user
    {
        loadProductsFromFile("products.txt");
        loadUsersFromFile("users.txt");
    }

    void loadProductsFromFile(const string &filename)
    {
        ifstream file(filename);
        if (!file)
        {
            cout << "Error opening product file.\n";
            return;
        }

        int id;
        string name;
        double price;

        while (file >> id >> ws)
        {
            getline(file, name, ',');
            file >> price;
            products.emplace_back(id, name, price);
        }

        file.close();
    }

    void loadUsersFromFile(const string &filename)
    {
        ifstream file(filename);
        if (!file)
        {
            cout << "Error opening user file.\n";
            return;
        }

        int id;
        string name;

        while (file >> id >> ws)
        {
            getline(file, name);
            users.emplace_back(id, name);
        }

        file.close();
    }

    void saveProductsToFile(const string &filename)
    {
        ofstream file(filename);
        if (!file)
        {
            cout << "Error opening file to save products.\n";
            return;
        }

        for (const auto &product : products)
        {
            file << product.id << "," << product.name << "," << product.price << "\n";
        }

        file.close();
    }

    void saveUsersToFile(const string &filename)
    {
        ofstream file(filename);
        if (!file)
        {
            cout << "Error opening file to save users.\n";
            return;
        }

        for (const auto &user : users)
        {
            file << user.id << "," << user.name << "\n";
        }

        file.close();
    }

    void addProduct(int id, const string &name, double price)
    {
        products.emplace_back(id, name, price);
        cout << "Product " << name << " added successfully.\n";
        saveProductsToFile("products.txt");
    }

    void addUser(int id, const string &name)
    {
        users.emplace_back(id, name);
        cout << "User " << name << " added successfully.\n";
        saveUsersToFile("users.txt");
    }

    void deleteUser(int id)
    {
        auto it = remove_if(users.begin(), users.end(), [id](User &user)
                            { return user.id == id; });
        if (it != users.end())
        {
            users.erase(it, users.end());
            cout << "User with ID " << id << " deleted successfully.\n";
            saveUsersToFile("users.txt");
        }
        else
        {
            cout << "User with ID " << id << " not found.\n";
        }
    }

    void setCurrentUser(int userId)
    {
        currentUserId = userId;
    }

    int getCurrentUserId() const { return currentUserId; } // Added getter for currentUserId

    void displayProducts()
    {
        cout << "\nAvailable Products:\n";
        cout << "ID\tName\t\tPrice\n";
        cout << "---------------------------------\n";
        for (const auto &product : products)
        {
            cout << product.id << "\t" << product.name << "\t\t" << product.price << endl;
        }
        cout << "---------------------------------\n";
    }

    void displayUsers() // New method to display all users
    {
        cout << "\nRegistered Users:\n";
        cout << "ID\tName\n";
        cout << "-------------------------\n";
        for (const auto &user : users)
        {
            cout << user.id << "\t" << user.name << endl;
        }
        cout << "-------------------------\n";
    }

    void addToCart(int productId, int quantity)
    {
        if (currentUserId == -1)
        {
            cout << "Please set a current user before adding to cart.\n";
            return;
        }

        bool found = false;

        for (const auto &product : products)
        {
            if (product.id == productId)
            {
                found = true;
                if (quantity <= 0)
                {
                    cout << "Invalid quantity. Please enter a positive number.\n";
                    return;
                }
                cart.emplace_back(product, quantity);
                cout << product.name << " added to cart.\n";

                // Display product and user information
                cout << "\nProduct Information:\n";
                cout << "ID: " << product.id << "\n";
                cout << "Name: " << product.name << "\n";
                cout << "Price: $" << fixed << setprecision(2) << product.price << "\n";
                cout << "Quantity: " << quantity << "\n";

                cout << "\nUser Information:\n";
                for (const auto &user : users)
                {
                    if (user.id == currentUserId)
                    {
                        cout << "ID: " << user.id << "\n";
                        cout << "Name: " << user.name << "\n";
                        break;
                    }
                }
                break;
            }
        }

        if (!found)
        {
            cout << "Invalid product ID. Please try again.\n";
        }
    }

    void saveBillToFile(const string &filename)
    {
        ofstream file(filename);
        if (!file)
        {
            cout << "Error opening file to save bill.\n";
            return;
        }

        double total = 0;
        file << "--- BILL ---\n";
        file << "User ID: " << currentUserId << "\n"; // Include user ID in the bill
        file << "ID\tName\t\tQuantity\tPrice\tSubtotal\n";
        file << "--------------------------------------------------\n";

        for (const auto &item : cart)
        {
            const Product &product = item.first;
            int quantity = item.second;
            double subtotal = product.price * quantity;
            total += subtotal;
            file << product.id << "\t" << product.name << "\t\t" << quantity
                 << "\t\t" << product.price << "\t" << subtotal << endl;
        }

        file << "--------------------------------------------------\n";
        file << "Total Amount: " << fixed << setprecision(2) << total << endl;
        file << "Thank you for shopping with us!\n";
        file.close();
    }
};

int main()
{
    BillingSystem system;
    int choice, quantity;

    cout << "--- Supermarket Billing System ---\n";

    while (true)
    {
        cout << "\n1. Display Products\n2. Add Product\n3. Add User\n4. Delete User\n5. Set Current User\n6. Add to Cart\n7. Display Users\n8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1)
        {
            system.displayProducts();
        }
        else if (choice == 2)
        {
            int id;
            string name;
            double price;
            cout << "Enter Product ID: ";
            cin >> id;
            cout << "Enter Product Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Product Price: ";
            cin >> price;
            system.addProduct(id, name, price);
        }
        else if (choice == 3)
        {
            int id;
            string name;
            cout << "Enter User ID: ";
            cin >> id;
            cout << "Enter User Name: ";
            cin.ignore();
            getline(cin, name);
            system.addUser(id, name);
        }
        else if (choice == 4)
        {
            int id;
            cout << "Enter User ID to delete: ";
            cin >> id;
            system.deleteUser(id);
        }
        else if (choice == 5)
        {
            int id;
            cout << "Enter User ID to set as current user: ";
            cin >> id;
            system.setCurrentUser(id);
            cout << "Current user set to ID " << id << ".\n";
        }
        else if (choice == 6)
        {
            if (system.getCurrentUserId() == -1)
            {
                cout << "Please set a current user before adding to cart.\n";
                continue;
            }

            system.displayProducts();
            cout << "Enter the product ID to add to cart (0 to finish): ";
            cin >> choice;

            if (choice == 0)
                continue;

            cout << "Enter quantity: ";
            cin >> quantity;

            system.addToCart(choice, quantity);
        }
        else if (choice == 7) // New option to display users
        {
            system.displayUsers();
        }
        else if (choice == 8)
        {
            break;
        }
        else
        {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}