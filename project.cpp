#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

// Forward declarations
class IMarketResearcher;

// Product class
class Product {
private:
    string name;
    string description;
    vector<string> reviews;
    double price;
    static int totalProducts;

public:
    // Constructor
    Product(const string& name, const string& description, double price) : name(name), description(description), price(price) {
        totalProducts++;
    }

    // Copy constructor
    Product(const Product& other) : name(other.name), description(other.description), reviews(other.reviews), price(other.price) {
        totalProducts++;
    }

    // Destructor
    ~Product() {
        totalProducts--;
    }

    // Getter methods
    const string& getName() const {
        return name;
    }

    double getPrice() const {
        return price;
    }

    static int getTotalProducts() {
        return totalProducts;
    }

    // Setter method
    void setPrice(double newPrice) {
        price = newPrice;
    }

    // Method to add a review to the product
    void addReview(const string& review) {
        reviews.push_back(review);
    }

    // Allowing access to private members for CompanyExecutive and Customer classes
    friend class CompanyExecutive;
    friend class Customer;
};

int Product::totalProducts = 0;

// User class
class User {
protected:
    string name;

public:
    User(const string& name) : name(name) {}
};

// Customer class, derived from User
class Customer : public User {
public:
    Customer(const string& name) : User(name) {}

    // Method to write a review for a product
    void writeReview(Product* product, const string& review) {
        product->addReview(formatReview(review));
    }

private:
    // Method to format the review with customer name
    string formatReview(const string& review) {
        return name + ": " + review;
    }
};

// Interface for MarketResearcher class
class IMarketResearcher {
public:
    virtual void addProduct(Product* product) = 0;
    virtual ~IMarketResearcher() {}
};

// MarketResearcher class, derived from IMarketResearcher
class MarketResearcher : public IMarketResearcher {
private:
    vector<Product*> products;

public:
    // Method to add a product to the researcher's list
    void addProduct(Product* product) {
        products.push_back(product);
    }
};

// CompanyExecutive class, derived from User and IMarketResearcher
class CompanyExecutive : public User, public IMarketResearcher {
private:
    vector<Product*> products;

public:
    CompanyExecutive(const string& name) : User(name) {}

    // Method to add a product to the executive's list
    void addProduct(Product* product) {
        products.push_back(product);
    }

    // Method to view all products and their details
    void viewProducts() {
        cout << "Products:" << endl;
        for (Product* product : products) {
            cout << "Name: " << product->getName() << endl;
            cout << "Description: " << product->description << endl;
            cout << "Price: $" << product->getPrice() << endl;
            cout << "Reviews:" << endl;
            for (const string& review : product->reviews) {
                cout << review << endl;
            }
            cout << endl;
        }
    }

    // Method to write product data to a CSV file
    void writeDataToCSV(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening file!" << endl;
            return;
        }

        // Write product data to CSV
        file << "Name,Description,Price,Review" << endl;
        for (Product* product : products) {
            file << product->getName() << ","
                << product->description << ","
                << product->getPrice() << ","
                << "" << endl; // Empty review for products

            for (const string& review : product->reviews) {
                file << ","
                    << ","
                    << ","
                    << review << endl; // Customer reviews for the product
            }
        }

        file.close();
        cout << "Data has been written to " << filename << endl;
    }

    // Method to change the price of a product
    void changePrice(Product* product, double newPrice) {
        product->setPrice(newPrice);
        cout << "Price of " << product->getName() << " has been changed to $" << product->getPrice() << endl;
        writeDataToCSV("data.csv");
    }

    // Method to print content of a competitor using a CSV file
    // Method to print content of a competitor using a CSV file
void printCompetitorContent(const string& filename) {
    string competitorFilename = filename + ".csv";
    ifstream file(competitorFilename);
    if (!file.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    cout << "Content of competitor's file:" << endl;
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }

    file.close();
}

};

int main() {
    // Create instances of the classes
    CompanyExecutive executive("Executive");
    MarketResearcher researcher;
    Product* product1 = new Product("Product 1", "Description 1", 9.99);
    Product* product2 = new Product("Product 2", "Description 2", 14.99);

    // Add products to the Company Executive and Market Researcher
    executive.addProduct(product1);
    executive.addProduct(product2);
    researcher.addProduct(product1);
    researcher.addProduct(product2);

    // Prompt for user type
    cout << "Are you a Customer or an Executive? (C/E): ";
    char userType;
    cin >> userType;
    cin.ignore();

    if (userType == 'C' || userType == 'c') {
        // User is a Customer
        string customerName;
        string review;
        char choice;

        do {
            cout << "Enter customer name: ";
            getline(cin, customerName);

            Customer customer(customerName);

            cout << "Enter review: ";
            getline(cin, review);

            int productNumber;
            while (true) {
                cout << "Enter product number (1 or 2): ";
                if (cin >> productNumber && (productNumber == 1 || productNumber == 2))
                    break;
                else {
                    cout << "Invalid input. Please enter 1 or 2." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            cin.ignore(); // Ignore the newline character

            Product* selectedProduct = (productNumber == 1) ? product1 : product2;
            customer.writeReview(selectedProduct, review);

            executive.writeDataToCSV("data.csv");

            cout << "Do you want to add another review? (Y/N): ";
            cin >> choice;
            cin.ignore(); // Ignore the newline character
        } while (choice == 'Y' || choice == 'y');
    } else if (userType == 'E' || userType == 'e') {
        // User is an Executive
        char option;
        do {
            cout << "Choose an option:" << endl;
            cout << "1. View products" << endl;
            cout << "2. Change product price" << endl;
            cout << "3. Print competitor's content" << endl;
            cout << "4. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> option;
            cin.ignore(); // Ignore the newline character

            switch (option) {
                case '1': {
                    executive.viewProducts();
                    break;
                }
                case '2': {
                    int productNumber;
                    while (true) {
                        cout << "Enter product number (1 or 2): ";
                        if (cin >> productNumber && (productNumber == 1 || productNumber == 2))
                            break;
                        else {
                            cout << "Invalid input. Please enter 1 or 2." << endl;
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                    }
                    cin.ignore(); // Ignore the newline character

                    Product* selectedProduct = (productNumber == 1) ? product1 : product2;

                    double newPrice;
                    cout << "Enter new price: $";
                    cin >> newPrice;
                    cin.ignore(); // Ignore the newline character

                    executive.changePrice(selectedProduct, newPrice);
                    break;
                }
                case '3': {
                    string competitorFilename;
                    cout << "Enter competitor's CSV filename: ";
                    getline(cin, competitorFilename);
                    executive.printCompetitorContent(competitorFilename);
                    break;
                }
                case '4': {
                    cout << "Exiting..." << endl;
                    break;
                }
                default: {
                    cout << "Invalid option. Please try again." << endl;
                    break;
                }
            }
        } while (option != '4');
    } else {
        cout << "Invalid user type. Exiting..." << endl;
    }

    // Cleanup
    delete product1;
    delete product2;

    return 0;
}