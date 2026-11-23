#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <ctime>
using namespace std;

string aMinusculas(string s) {
    for (char& c : s) c = (char)tolower((unsigned char)c);
    return s;
}

string recortar(const string& s) {
    size_t ini = s.find_first_not_of(" \t\r\n");
    if (ini == string::npos) return "";
    size_t fin = s.find_last_not_of(" \t\r\n");
    return s.substr(ini, fin - ini + 1);
}

string leerTexto(const string& mensaje) {
    string linea;
    while (true) {
        cout << mensaje;
        if (!getline(cin, linea)) exit(0);
        linea = recortar(linea);
        if (!linea.empty()) return linea;
        cout << "  ! El valor no puede estar vacío.\n";
    }
}

int leerEntero(const string& mensaje, int minimo, int maximo) {
    while (true) {
        string linea = leerTexto(mensaje);
        try {
            size_t pos;
            int valor = stoi(linea, &pos);
            if (pos == linea.size() && valor >= minimo && valor <= maximo) return valor;
        }
        catch (...) {}
        cout << "  ! Ingresa un número entero entre " << minimo << " y " << maximo << ".\n";
    }
}

double leerDecimal(const string& mensaje, double minimo) {
    while (true) {
        string linea = leerTexto(mensaje);
        for (char& c : linea) if (c == ',') c = '.';
        try {
            size_t pos;
            double valor = stod(linea, &pos);
            if (pos == linea.size() && valor >= minimo) return valor;
        }
        catch (...) {}
        cout << "  ! Ingresa un número mayor o igual a " << minimo << ".\n";
    }
}

bool leerSiNo(const string& mensaje) {
    while (true) {
        string r = aMinusculas(leerTexto(mensaje + " (s/n): "));
        if (r == "s" || r == "si" || r == "sí") return true;
        if (r == "n" || r == "no") return false;
        cout << "  ! Responde 's' o 'n'.\n";
    }
}

void pausar() {
    cout << "\nPresiona Enter para continuar...";
    string tmp;
    if (!getline(cin, tmp)) exit(0);
}

string horaActual() {
    time_t t = time(nullptr);
    tm info;
#ifdef _WIN32
    localtime_s(&info, &t);
#else
    localtime_r(&t, &info);
#endif
    char buf[20];
    strftime(buf, sizeof(buf), "%H:%M:%S", &info);
    return buf;
}

string dinero(double valor) {
    ostringstream os;
    os << fixed << setprecision(2) << valor;
    return os.str();
}

string columna(const string& s, size_t ancho) {
    string res;
    size_t visibles = 0;
    for (size_t i = 0; i < s.size() && visibles < ancho;) {
        unsigned char c = s[i];
        size_t len = (c < 0x80) ? 1 : ((c >> 5) == 6) ? 2 : ((c >> 4) == 14) ? 3 : 4;
        res += s.substr(i, len);
        i += len;
        visibles++;
    }
    res.append(ancho - visibles, ' ');
    return res;
}

string derecha(const string& s, size_t ancho) {
    return s.size() >= ancho ? s : string(ancho - s.size(), ' ') + s;
}

void titulo(const string& texto) {
    cout << "\n==================================================\n  " << texto
        << "\n==================================================\n";
}

struct Producto {
    int codigo = 0;
    string nombre;
    string categoria;
    double precio = 0.0;
    bool disponible = true;
};

void encabezadoProductos() {
    cout << columna("Código", 8) << columna("Nombre", 24) << columna("Categoría", 16)
        << derecha("Precio", 10) << "  Disponible\n"
        << string(70, '-') << "\n";
}

void mostrarProducto(const Producto& p) {
    cout << columna(to_string(p.codigo), 8) << columna(p.nombre, 24) << columna(p.categoria, 16)
        << derecha(dinero(p.precio), 10) << "  " << (p.disponible ? "Sí" : "No") << "\n";
}

void fichaProducto(const Producto& p) {
    cout << "  Código      : " << p.codigo << "\n"
        << "  Nombre      : " << p.nombre << "\n"
        << "  Categoría   : " << p.categoria << "\n"
        << "  Precio      : " << dinero(p.precio) << "\n"
        << "  Disponible  : " << (p.disponible ? "Sí" : "No") << "\n";
}

class ArregloProductos {
    Producto* datos;
    int tamano;
    int capacidad;

    void redimensionar() {
        capacidad *= 2;
        Producto* nuevo = new Producto[capacidad];
        for (int i = 0; i < tamano; i++) nuevo[i] = datos[i];
        delete[] datos;
        datos = nuevo;
    }

public:
    explicit ArregloProductos(int capacidadInicial = 4)
        : datos(new Producto[capacidadInicial]), tamano(0), capacidad(capacidadInicial) {
    }
    ~ArregloProductos() { delete[] datos; }
    ArregloProductos(const ArregloProductos&) = delete;
    ArregloProductos& operator=(const ArregloProductos&) = delete;

    void agregar(const Producto& p) {
        if (tamano == capacidad) redimensionar();
        datos[tamano++] = p;
    }
    int getTamano() const { return tamano; }
    int getCapacidad() const { return capacidad; }
    bool estaVacio() const { return tamano == 0; }
    Producto& obtener(int i) { return datos[i]; }
    void intercambiar(int i, int j) {
        Producto tmp = datos[i];
        datos[i] = datos[j];
        datos[j] = tmp;
    }
};

int main() {
    return 0;
}