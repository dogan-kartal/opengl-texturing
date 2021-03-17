#include <Windows.h> //Windows komutları için başlık dosyası
#include <stdlib.h>  //Standart kütüphaneleri içeren başlık dosyası
#include <GL/glut.h> //Glut için başlık dosyası 
#include <assert.h> /*program tarafından yapılan varsayımları doğrulamak ve
bu varsayım yanlışsa bir tanılama iletisi yazdırmak için kullanılır*/
#include <fstream> //Dosyalar üzerinde çalışabilen input-output sınıfı
using namespace std; // std isim uzayını standart olarak kullnamak için

GLfloat aciPiramit = 0.0f; //Piramidin dönme açısı
GLfloat aciKup = 0.0f;     //Küpün dönme açısı
GLfloat xgoturKup = -2.0f, ygoturKup = 0.0f, zgoturKup = -7.0f; //Küp için hareket değişkenleri
GLfloat xgoturPiramit = 1.5f, ygoturPiramit = 0.0f, zgoturPiramit = -6.0f; //Piramit için hareket değişkenleri
int t_yenileme = 20; //Milisaniye cinsinden yenileme aralığı

class Image {    //Kedi ve Köpek resimlerini programın içinde kullanabilmek için bir sınıf oluşturuyoruz
public:
	Image(char* ps, int w, int h); //Resimlerin piksel , yükseklik ve genişlik değerleri olmalı
	Image(); // Fonksiyon overloading yapılıyor
	//Gerekli değişkenler oluşturuluyor
	char* pixels;
	int width;
	int height;
};

Image* loadBMP(const char* filename); //Resim BMP formatında yükleniyor




GLuint loadTexture(Image* image) { //Resimleri Texture'a dönüştürmek için class
	GLuint textureId;
	glGenTextures(1, &textureId); //Texture yaratan kod
	glBindTexture(GL_TEXTURE_2D, textureId); //.BMP resmini kullanarak texture üretiyoruz
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	//2 Boyutlu bir texture oluşturan kod
	return textureId;
}

GLuint* textures = new GLuint[2]; //2 tane texture tutmak için
Image* image[2]; //resimleri tutmak için

void Ayarlar() {

	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);/* Mavi renk penceremizin
	arka alan rengidir; aslında ekranın hangi renkle
	temizleneceğini belirtir.*/
	glClearDepth(1.0f);/*glClearDepth fonksiyonunu 1.0 değeri ile çağırarak derinlik
	tamponunun temizlenmesini sağlıyoruz. 1.0 ile Arka plan derinliğini en uzağa ayarlanıyor*/
	glEnable(GL_DEPTH_TEST);//z-ekseni için derinlik testi
	/* Derinlik testinin yapılabilmesi için GL_DEPTH_TEST sembolik sabitini, glEnable fonksiyonuna
	parametre olarak geçerek çağırıyoruz.*/
	glDepthFunc(GL_LEQUAL);// Karmaşık grafiklerde Derinlik testi için kullanılır:
	/*Daha sonra glDepthFunc fonksiyonu ile derinlik testinde kullanılacak fonksiyonu belirtiyoruz.
	Burada parametre olarak geçilen GL_LEQUAL sembolik sabitinin belirttiği derinlik testi
	fonksiyonu, o an gelen z değeri, tamponda saklanan z değerine eşit ise veya ondan daha küçükse
	başarılı olur.*/
	glShadeModel(GL_SMOOTH);// karmaşık grafiklerde Düzgün gölgelendirmeyi etkinleştirir
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);/* karmaşık grafiklerde perspektif
	ayarlamalarını(düzeltmelerini) yapar.*/

	image[0] = loadBMP("kedi.bmp"); //kedi resmini image'e atıyoruz
	textures[0] = loadTexture(image[0]); // image'i texture'a çeviriyourz
	delete image[0];//ve image'i siliyoruz

	image[1] = loadBMP("kopek.bmp");// köpek resmini image'e atıyoruz
	textures[1] = loadTexture(image[1]);// image'i texture'a çeviriyourz
	delete image[1];//ve image'i siliyoruz

}

void CizimFonksiyonu() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);/* belirli bir bellek tamponunu veya
	tampon bileşenlerini silmek icin kullanilmaktadir.Tampon ile kastimiz bellekte ki bir is icin
	ayrılmış belirli bir hafiza parcasidir. Bir cizimin kirmizi,yesil,ve mavi bileseleri renk tamponu
	ve ya piksel tamponu olarak adlandirilir.*/
	glMatrixMode(GL_MODELVIEW);/* Kameranın pozisyonu ve cisimlerin 3D geometrisi ile ilgili
	çizme, görüntüleme, konumlandırma ve yönlendirme işlemlerinden önce matrislerin hangi işlem için
	düzenleneceği belirtilmelidir: bu örnekte -->model-view matrisi kulanılıyor*/



	//piramit
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glLoadIdentity();
	glTranslatef(xgoturPiramit, ygoturPiramit, zgoturPiramit);
	glRotatef(aciPiramit, 1.0f, 1.0f, 0.0f);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_TRIANGLES);
	//ön
	glTexCoord2f(0.0f, 0.0f);     //Texture Kordinatlarını belirtiyoruz
	glVertex3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);    //Texture Kordinatlarını belirtiyoruz
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);    //Texture Kordinatlarını belirtiyoruz
	glVertex3f(1.0f, -1.0f, 1.0f);

	//sag
	glTexCoord2f(0.0f, 0.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);   //Texture Kordinatlarını belirtiyoruz
	glVertex3f(1.0f, -1.0f, -1.0f);

	//arka
	glTexCoord2f(0.0f, 0.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(-1.0f, -1.0f, -1.0f);

	//sol
	glTexCoord2f(0.0f, 0.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();

	//küp
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glLoadIdentity();
	glTranslatef(xgoturKup, ygoturKup, zgoturKup);
	glRotatef(aciKup, 1.0f, 1.0f, 0.0f);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);
	//üst
	glTexCoord2f(0.0f, 1.0f); //Texture Kordinatlarını belirtiyoruz
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); //Texture Kordinatlarını belirtiyoruz
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); //Texture Kordinatlarını belirtiyoruz
	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); //Texture Kordinatlarını belirtiyoruz
	glVertex3f(1.0f, 1.0f, -1.0f);

	//alt
	glTexCoord2f(1.0f, 1.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(-1.0f, -1.0f, 1.0f);

	//ön
	glTexCoord2f(0.0f, 0.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(-1.0f, 1.0f, 1.0f);

	//arka
	glTexCoord2f(1.0f, 0.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(1.0f, -1.0f, -1.0f);

	//sol
	glTexCoord2f(0.0f, 0.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(-1.0f, 1.0f, -1.0f);

	//sag
	glTexCoord2f(1.0f, 0.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);  //Texture Kordinatlarını belirtiyoruz
	glVertex3f(1.0f, -1.0f, 1.0f);
	glEnd();

	glutSwapBuffers();/*Çerçeveyi çizdikten sonra Ön ve arka tamponlar değişir. Burada, biri
	gösterilen diğeri gizlenen, çift tampon modunda pencere kullanıyoruz. Bu durumda OpenGL
	komutlarının görüntülenmesi daima gizli tampon içine yapılır. glutSwapBuffers(), çağrıldığında,
	tamponları değiştokuş ederek çizileni pencere içinde görüntüler. Tercih edilmesinin nedeni,
	çerçeve içeriğinin satır satır oluşumunu insan gözünden saklamasıdır*/


	aciPiramit += 1.0f;
	aciKup -= 1.0f;

}

void GorunumAyari(GLsizei x, GLsizei y) {

	if (y == 0) y = 1;
	GLfloat oran = (GLfloat)x / (GLfloat)y;

	glViewport(0, 0, x, y);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(45.0f, oran, 1.0f, 20.0f);

}

void Zamanlayici(int value) {

	glutPostRedisplay(); //CizimFonksiyonu()'nun yaptığı işlem sonrası güncelleme için
	glutTimerFunc(t_yenileme, Zamanlayici, 0); // next timer call milliseconds later

}
//Hareket Fonksiyonları
void gotursagKup(void) {
	glLoadIdentity();
	xgoturKup += 0.1f;
}

void gotursolKup(void) {
	glLoadIdentity();
	xgoturKup -= 0.1f;
}

void goturyukariKup(void) {
	glLoadIdentity();
	ygoturKup += 0.1f;
}

void goturasagiKup(void) {
	glLoadIdentity();
	ygoturKup -= 0.1f;
}

void goturileriKup(void) {
	glLoadIdentity();
	zgoturKup -= 0.1f;
}

void goturgeriKup(void) {
	glLoadIdentity();
	zgoturKup += 0.1f;
}

void gotursagPiramit(void) {
	glLoadIdentity();
	xgoturPiramit += 0.1f;
}

void gotursolPiramit(void) {
	glLoadIdentity();
	xgoturPiramit -= 0.1f;
}

void goturyukariPiramit(void) {
	glLoadIdentity();
	ygoturPiramit += 0.1f;
}

void goturasagiPiramit(void) {
	glLoadIdentity();
	ygoturPiramit -= 0.1f;
}

void goturileriPiramit(void) {
	glLoadIdentity();
	zgoturPiramit -= 0.1f;
}

void goturgeriPiramit(void) {
	glLoadIdentity();
	zgoturPiramit += 0.1f;
}

void klavye(int dugme, int x, int y) {
	switch (dugme) {
	case GLUT_KEY_LEFT:gotursolKup(); break;
	case GLUT_KEY_RIGHT:gotursagKup(); break;
	case GLUT_KEY_UP:goturyukariKup(); break;
	case GLUT_KEY_DOWN:goturasagiKup(); break;
	case GLUT_KEY_PAGE_UP:goturileriKup(); break;
	case GLUT_KEY_PAGE_DOWN:goturgeriKup(); break;
	}
	glutPostRedisplay();
}

void klavye(unsigned char dugme, int x, int y) {
	switch (dugme) {
	case 'a':gotursolPiramit(); break;
	case'd':gotursagPiramit(); break;
	case'w':goturyukariPiramit(); break;
	case's':goturasagiPiramit(); break;
	case'+':goturileriPiramit(); break;
	case'-':goturgeriPiramit(); break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);

	glutInitWindowPosition(300, 125);
	glutInitWindowSize(700, 500);
	glutCreateWindow("201613171010");

	glutDisplayFunc(CizimFonksiyonu);
	glutReshapeFunc(GorunumAyari);
	Ayarlar();
	glutTimerFunc(0, Zamanlayici, 0);
	glutSpecialFunc(klavye);
	glutKeyboardFunc(klavye);
	glutMainLoop();
	return 0;

}

Image::Image(char* ps, int w, int h) : pixels(ps), width(w), height(h) {

}

Image::Image() {
	delete[] pixels;
}

namespace {
	int toInt(const char* bytes) {   //Dört karakterli bir diziyi tam sayıya dönüştüren kod
		return (int)(((unsigned char)bytes[3] << 24) |
			((unsigned char)bytes[2] << 16) |
			((unsigned char)bytes[1] << 8) |
			(unsigned char)bytes[0]);
	}


	short toShort(const char* bytes) {  //İki karakterli bir diziyi short türüne dönüştüren kod
		return (short)(((unsigned char)bytes[1] << 8) |
			(unsigned char)bytes[0]);
	}


	int readInt(ifstream& input) {  //Sonraki 4 byte'ı tam sayı olarak okur
		char buffer[4];
		input.read(buffer, 4);
		return toInt(buffer);
	}


	short readShort(ifstream& input) { //Sonraki 2 byte'ı tam sayı olarak okur
		char buffer[2];
		input.read(buffer, 2);
		return toShort(buffer);
	}


	template<class T>
	class auto_array {
	private:
		T* array;
		mutable bool isReleased;
	public:
		explicit auto_array(T* array_ = NULL) :
			array(array_), isReleased(false) {
		}

		auto_array(const auto_array<T>& aarray) {
			array = aarray.array;
			isReleased = aarray.isReleased;
			aarray.isReleased = true;
		}

		~auto_array() {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
		}

		T* get() const {
			return array;
		}

		T& operator*() const {
			return *array;
		}

		void operator=(const auto_array<T>& aarray) {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
			array = aarray.array;
			isReleased = aarray.isReleased;
			aarray.isReleased = true;
		}

		T* operator->() const {
			return array;
		}

		T* release() {
			isReleased = true;
			return array;
		}

		void reset(T* array_ = NULL) {
			if (!isReleased && array != NULL) {
				delete[] array;
			}
			array = array_;
		}

		T* operator+(int i) {
			return array + i;
		}

		T& operator[](int i) {
			return array[i];
		}
	};
}

Image* loadBMP(const char* filename) { //Bitmap dosyasını yüklüyoruz
	ifstream input;
	input.open(filename, ifstream::binary);
	assert(!input.fail() || !"Dosya Bulunamadı"); //Eğer dosyayı bulamazsa hata vermesi için
	char buffer[2];
	input.read(buffer, 2);
	assert(buffer[0] == 'B' && buffer[1] == 'M' || !"Bitmap dosyası değil"); //Eğer dosya bitmap değilse hata verecek
	input.ignore(8);
	int dataOffset = readInt(input);

	//Header'ları oku
	int headerSize = readInt(input);
	int width;
	int height;
	switch (headerSize) {
	case 40:
		//V3
		width = readInt(input);
		height = readInt(input);
		input.ignore(2);
		assert(readShort(input) == 24 || !"Piksel başına görüntü 24 bit değil"); //resim 24 bit değilse hata verecek
		assert(readShort(input) == 0 || !"Görüntü sıkıştırıldı");
		break;
	case 12:
		//OS/2 V1
		width = readShort(input);
		height = readShort(input);
		input.ignore(2);
		assert(readShort(input) == 24 || !"Piksel başına görüntü 24 bit değil");
		break;
	case 64:
		//OS/2 V2
		assert(!"OS/2 V2 bitmap'leri yüklenemiyor");
		break;
	case 108:
		//Windows V4
		assert(!"Windows V4 bitmap'leri yüklenemiyor");
		break;
	case 124:
		//Windows V5
		assert(!"Windows V5 bitmap'leri yüklenemiyor");
		break;
	default:
		assert(!"Tanımlanamayan Bitmap formatı");
	}

	//Veriyi okumak için
	int bytesPerRow = ((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
	int size = bytesPerRow * height;
	auto_array<char> pixels(new char[size]);
	input.seekg(dataOffset, ios_base::beg);
	input.read(pixels.get(), size);

	//Veriyi doğru formata dönüştürüyor
	auto_array<char> pixels2(new char[width * height * 3]);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int c = 0; c < 3; c++) {
				pixels2[3 * (width * y + x) + c] =
					pixels[bytesPerRow * y + 3 * x + (2 - c)];
			}
		}
	}

	input.close();
	return new Image(pixels2.release(), width, height);
}

