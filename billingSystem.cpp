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

// Class to manage the billing system
class BillingSystem
{
private:
    vector<Product> products;        // Available products
    vector<pair<Product, int>> cart; // Cart with product and quantity

public:
    // Constructor to initialize available products (load from file)
    BillingSystem()
    {
        loadProductsFromFile("products.txt");
    }

    // Load products from file
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

    // Save products to file
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

    // Add product to product list
    void addProduct(int id, const string &name, double price)
    {
        products.emplace_back(id, name, price);
        cout << "Product " << name << " added successfully.\n";
        saveProductsToFile("products.txt"); // Save the updated product list
    }

    // Display available products
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

    // Add product to cart
    void addToCart(int productId, int quantity)
    {
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
                break;
            }
        }

        if (!found)
        {
            cout << "Invalid product ID. Please try again.\n";
        }
    }

    // Save the final bill to a file
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
        cout << "\n1. Display Products\n2. Add Product\n3. Add to Cart\n4. Exit\n";
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
            system.displayProducts();
            cout << "Enter the product ID to add to cart (0 to finish): ";
            cin >> choice;

            if (choice == 0)
                break;

            cout << "Enter quantity: ";
            cin >> quantity;

            system.addToCart(choice, quantity);
        }
        else if (choice == 4)
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