#include <iostream>
#include <string>
using namespace std;

// REQUIREMENT: ABSTRACTION (3-4 Abstract Classes with Pure Virtual Functions)
class BaseComponents {
public:
  virtual void viewDetails() const = 0; // Pure Virtual
  virtual ~BaseComponents() {}
};

class RegulatoryAudit {
public:
  virtual void performVerification() = 0; // Pure Virtual
  virtual ~RegulatoryAudit() {}
};

class Tax {
public:
  virtual double calculateTaxRate() const = 0; // Pure Virtual
  virtual ~Tax() {}
};

class Authorization {
public:
  virtual bool verifyCredentials(string inputPass) = 0; // Pure Virtual
  virtual ~Authorization() {}
};

// REQUIREMENT: COMPOSITION
class Engine {
  string serialNo;
  int hp;
  float cc;

public:
  Engine(string id = "V4", int hp = 150, float cc = 2.0)
      : serialNo(id), hp(hp), cc(cc) {}

  void displaySpecifications() const {
    cout << " Engine: " << serialNo << " | Horse Power: " << hp;
  }
};

// REQUIREMENT: INHERITANCE (User Hierarchy) & STATIC MEMBERS
class BaseUser : public BaseComponents, public Authorization {
protected:
  static int UsersCount; // STATIC MEMBER
  int ID;
  string fullName, email, Password;

public:
  BaseUser(int id, string name, string email, string pass)
      : ID(id), fullName(name), email(email), Password(pass) {
    UsersCount++;
  }

  static int getActiveUsers() { return UsersCount; } // STATIC FUNCTION
  virtual void displaySystemRole() const = 0;

  // REQUIREMENT: POLYMORPHISM (Function Overriding)
  bool verifyCredentials(string inputPass) override {
    return Password == inputPass;
  }

  // REQUIREMENT: FRIEND FUNCTION 1
  friend void administrativePasswordReset(BaseUser &targetUser, string newPass);
};
int BaseUser::UsersCount = 0;

// REQUIREMENT: VEHICLE HIERARCHY & OPERATOR OVERLOADING
class BaseVehicle : public BaseComponents, public Tax {
protected:
  const string vehicleIdNo; // CONSTANT MEMBER
  string manufacturer, model;
  double Price;
  Engine engine; // COMPOSITION (Vehicle HAS A Engine)

public:
  BaseVehicle(string vin, string make, string model, double price, Engine eng)
      : vehicleIdNo(vin), manufacturer(make), model(model), Price(price),
        engine(eng) {}

  string getVIN() const { return vehicleIdNo; }

  // REQUIREMENT: OPERATOR OVERLOADING (4 Operators: ==, <, +, >)
  bool operator==(const BaseVehicle &otherVehicle) const {
    return vehicleIdNo == otherVehicle.vehicleIdNo;
  }
  bool operator<(const BaseVehicle &otherVehicle) const {
    return Price < otherVehicle.Price;
  }
  double operator+(const BaseVehicle &otherVehicle) const {
    return Price + otherVehicle.Price;
  }
  bool operator>(const BaseVehicle &otherVehicle) const {
    return Price > otherVehicle.Price;
  }

  void viewDetails() const override {
    cout << endl << manufacturer << " " << model << " " << Price;
    engine.displaySpecifications();
  }

  // REQUIREMENT: FRIEND FUNCTION 2
  friend void applyDiscount(BaseVehicle &targetVehicle, double discountAmount);
};

// REQUIREMENT: DERIVED CLASSES (Inheritance & Polymorphism)
class Car : public BaseVehicle {
public:
  Car(string vin, string make, string model, double price, Engine eng)
      : BaseVehicle(vin, make, model, price, eng) {}

  double calculateTaxRate() const override { return Price * 0.12; }
};

class Bike : public BaseVehicle {
public:
  Bike(string vin, string make, string model, double price, Engine eng)
      : BaseVehicle(vin, make, model, price, eng) {}

  double calculateTaxRate() const override { return Price * 0.05; }
};

class Buyer : public BaseUser {
public:
  Buyer(int id, string name, string email, string pass)
      : BaseUser(id, name, email, pass) {}

  void viewDetails() const override { cout << "Buyer Profile: " << fullName; }
  void displaySystemRole() const override {
    cout << "Privilege: Purchasing Access\n";
  }
};

class Admin : public BaseUser {
  int clearanceLevel;

public:
  Admin(int id, string name, string email, string pass, int level)
      : BaseUser(id, name, email, pass), clearanceLevel(level) {}

  void viewDetails() const override { cout << "Administrator: " << fullName; }
  void displaySystemRole() const override {
    cout << "Privilege: System Architecture & Audit\n";
  }

  // REQUIREMENT: FRIEND FUNCTION 3
  friend void verifyAdminClearance(const Admin &targetAdmin);
};

// REQUIREMENT: AGGREGATION & ARRAYS OF OBJECTS
class MarketplaceListing {
  static int globalListingCount;
  BaseVehicle
      *registeredVehicle; // AGGREGATION (Listing HAS A pointer to Vehicle)

public:
  MarketplaceListing(BaseVehicle *vehiclePtr) : registeredVehicle(vehiclePtr) {
    globalListingCount++;
  }

  static int getTotalAds() { return globalListingCount; }
  BaseVehicle *getVehicleReference() { return registeredVehicle; }
};
int MarketplaceListing::globalListingCount = 0;

class BambrolistanWheelsMarketplace {
  MarketplaceListing *activeListings[20]; // ARRAY OF OBJECTS (Aggregation)
  int currentListingCount;

public:
  BambrolistanWheelsMarketplace() : currentListingCount(0) {}

  void postNewListing(MarketplaceListing *newAd) {
    if (currentListingCount < 20)
      activeListings[currentListingCount++] = newAd;
  }

  void filterationByVIN(string targetVIN) {
    for (int i = 0; i < currentListingCount; i++) {
      if (activeListings[i]->getVehicleReference()->getVIN() == targetVIN)
        activeListings[i]->getVehicleReference()->viewDetails();
    }
  }
};

// FRIEND FUNCTION IMPLEMENTATIONS (Accessing Private Members)
void administrativePasswordReset(BaseUser &targetUser, string newPass) {
  targetUser.Password = newPass; // Directly accessing private Password
}

void applyDiscount(BaseVehicle &targetVehicle, double discountAmount) {
  targetVehicle.Price -= discountAmount; // Directly accessing private Price
}

void verifyAdminClearance(const Admin &targetAdmin) {
  cout << "Security Audit: " << targetAdmin.fullName
       << " Operating at Level: " << targetAdmin.clearanceLevel
       << endl; // Accessing private clearanceLevel
}

// MAIN EXECUTION
int main() {
  Engine performanceEngine("V6-TURBO", 350, 3.5);
  Engine hawaiJahaz("W-16", 1000, 25000);

  Car Civic("123", "Honda", "Civic", 8500000, performanceEngine);
  Bike CD70("34(Poyon Oil)", "Honda", "CD-70", 18000000, hawaiJahaz);

  BambrolistanWheelsMarketplace platform;
  MarketplaceListing ad1(&Civic), ad2(&CD70);

  platform.postNewListing(&ad1);
  platform.postNewListing(&ad2);

  // Using Overloaded Operator >
  if (CD70 > Civic) {
    cout << ">>> Premium Category Vehicle Detected\n";
  }

  // Using Overloaded Operator +
  cout << "Total Portfolio Value: " << (Civic + CD70) << endl;

  Admin currentCEO(1, "Muhammad Yahya Sohail", "K250078@nu.edu.pk",
                   "mainNhiBataunga", 100);

  // Using Friend Function
  verifyAdminClearance(currentCEO);

  // Using Friend Function
  applyDiscount(CD70, 10000);

  cout << "Akhri Bhai wali Price: ";
  CD70.viewDetails();
  cout << endl;

  return 0;
}
