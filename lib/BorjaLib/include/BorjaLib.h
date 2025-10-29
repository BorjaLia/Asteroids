#pragma once
#include <iostream>
#include "conio.h"

//#include "windows.h"

#include <fstream>
#include <string>

#define HAS_SIGIL
#define HAS_RAYLIB

#ifdef HAS_RAYLIB
#include "raylib.h" 
#endif

#ifdef HAS_SIGIL
#include "sl.h"
#endif

//constexpr float PI = 3.14159265359;

struct bColor {
	unsigned char r = 255;
	unsigned char g = 255;
	unsigned char b = 255;
	unsigned char a = 255;
};

namespace ctrl {

	enum class ButtonMouse {
		NULL_BUTTON = -1,
		LMB,
		RMB,
		MMB,
		BWB,
		FWB,
		SIZE
	};

	enum class Key {

		NULL_KEY = 0,
		APOSTROPHE = 39,
		COMMA = 44,
		MINUS = 45,
		PERIOD = 46,
		SLASH = 47,
		ZERO = 48,
		ONE = 49,
		TWO = 50,
		THREE = 51,
		FOUR = 52,
		FIVE = 53,
		SIX = 54,
		SEVEN = 55,
		EIGHT = 56,
		NINE = 57,
		SEMICOLON = 59,
		EQUAL = 61,
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,
		LEFT_BRACKET = 91,
		BACKSLASH = 92,
		RIGHT_BRACKET = 93,
		GRAVE = 96,
		SPACE = 32,
		ESCAPE = 256,
		ENTER = 257,
		TAB = 258,
		BACKSPACE = 259,
		INSERT = 260,
		DELETE = 261,
		RIGHT = 262,
		LEFT = 263,
		DOWN = 264,
		UP = 265,
		PAGE_UP = 266,
		PAGE_DOWN = 267,
		HOME = 268,
		END = 269,
		CAPS_LOCK = 280,
		SCROLL_LOCK = 281,
		NUM_LOCK = 282,
		PRINT_SCREEN = 283,
		PAUSE = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		LEFT_SHIFT = 340,
		LEFT_CONTROL = 341,
		LEFT_ALT = 342,
		LEFT_SUPER = 343,
		RIGHT_SHIFT = 344,
		RIGHT_CONTROL = 345,
		RIGHT_ALT = 346,
		RIGHT_SUPER = 347,
		KB_MENU = 348,
		KP_0 = 320,
		KP_1 = 321,
		KP_2 = 322,
		KP_3 = 323,
		KP_4 = 324,
		KP_5 = 325,
		KP_6 = 326,
		KP_7 = 327,
		KP_8 = 328,
		KP_9 = 329,
		KP_DECIMAL = 330,
		KP_DIVIDE = 331,
		KP_MULTIPLY = 332,
		KP_SUBTRACT = 333,
		KP_ADD = 334,
		KP_ENTER = 335,
		KP_EQUAL = 336,
		SIZE = 349
	};

	bool IsMouseDown(ButtonMouse mouseButton);
	bool IsMousePressed(ButtonMouse mouseButton);
	ButtonMouse GetMouseButtonDown();

	bool IsKeyHeldDown(Key key);
	bool IsKeyPressed(Key key);
	Key GetKeyDown();
	char LastKeyPressed();										// returns last key press. false if none

	extern ctrl::Key lastKeyPress;
	extern ctrl::ButtonMouse lastMouseButton;
	extern ctrl::ButtonMouse menuSelectionMouseButton;
}

namespace utl {

	std::string SearchInFile(const char* filename, const char* search);
	bool SaveToFile(const char* filename, const char* filter, std::string info);
	bool SaveToFile(const char* filename, const char* filter, int info);

	extern std::string settingsFilePath;
}

namespace mth {

	void Clamp(float& num, float min = 0, float max = 1.0f);		// clamp number between min and max
	float Clamped(float num, float min = 0, float max = 1.0f);		// clamp number between min and max
	float Abs(float& num);										// makes the number positive
	float AsAbs(float num);										// makes the number positive

	float Min(float num1, float num2);
	float Max(float num1, float num2);

	float RadianToDegree(float angle);							// radians to degrees
	float DegreeToRadian(float angle);							// degrees to radians

	float Normalize(float& num, float min = 0, float max = 1.0f);
	float Normalized(float num, float min = 0, float max = 1.0f);

	float Lerp(float min, float max, float percentage = 0.5f);

	float WrapToRange(float num, float min = 0, float max = 1.0f);
	float MapToRange(float num, float iMin, float iMax, float oMin, float oMax);


	void SetRandomSeed(time_t seed = 0);													// set random seed
	int GetRandom(int min = 0, int max = 100);												// get random int
	float GetRandomf(float min = 0.0f, float max = 1.0f);		// get random float
}

namespace vec {

	struct Vector3 {
	private:

		bool isCartesianBool = true;
		bool isSphericalBool = false;

	public:

		float x;
		float y;
		float z;

		Vector3(bool cartesian = true);
		Vector3(float x, float y, float z, bool cartesian = true);
	};

	struct Vector2 {

	private:

		bool isCartesianBool = true;
		bool isCircularBool = false;

	public:

		float x;
		float y;

		Vector2(bool cartesian = true);
		Vector2(float x, float y, bool cartesian = true);

		void normalize();											// normalize the vector
		vec::Vector2 normalized();									// returns the normalized vector without modyfing the original
		float magnitude();											// returns the magnitude
		float magnitude(float magnitude);							// scale the vector and returns the new magnitude
		float determinant(Vector2 v);								// determinant between 2 vectors
		void clamp(float max);										// clamp the magnitude to a max value
		void clamp(float min, float max);							// clamp the magnitude between values
		float angle(Vector2 v = { 1,0 });							// angle between 2 vectors. default is {1,0}
		void rotate(float angle);									// rotate the vector by the given angle in radians
		vec::Vector2 rotated(float angle);							// return the rotated vector by the given angle in radians without modyfing the original
		void rotateDegree(float angle);								// rotate the vector by the given angle in degrees
		vec::Vector2 rotatedDegree(float angle);					// return the rotated vector by the given angle in degrees without modyfing the original
		void randomizeAngle();											// randomize the direction of the vector. keep magnitude
		void randomizeAngle(float min,float max);											// randomize the direction of the vector. keep magnitude
		void randomize(float maxMagnitude);							// randomize the direction of the vector. randomize magnitude between 1 and max
		void randomize(float minMagnitude, float maxMagnitude);		// randomize the direction of the vector. randomize magnitude between min and max

		bool isCartesian();
		bool setAsCartesian();
		bool isCircular();
		bool setAsCircular();

		vec::Vector2 asCircular();									// from cartesian (in radians)
		vec::Vector2 asCircularDegree();							// from cartesian (in degrees)
		vec::Vector2 toCircular();									// transform to circular from cartesian (in radians)
		vec::Vector2 toCircularDegree();							// transform to circular from cartesian (in degrees)

		vec::Vector2 asCartesian();									// from circular (in radians)
		vec::Vector2 asCartesianDegree();							// from circular (in degrees)
		vec::Vector2 toCartesian();									// to cartesian from circular (in radians)
		vec::Vector2 toCartesianDegree();							// to cartesian from circular (in degrees)

		vec::Vector2 operator + (const vec::Vector2 vector) {
			return vec::Vector2(x + vector.x, y + vector.y);
		}
		vec::Vector2 operator +=  (const vec::Vector2 vector) {
			this->x += vector.x;
			this->y += vector.y;
			return *this;
		}
		vec::Vector2 operator - (const vec::Vector2 vector) {
			return vec::Vector2(x - vector.x, y - vector.y);
		}
		vec::Vector2 operator -= (const vec::Vector2 vector) {
			this->x -= vector.x;
			this->y -= vector.y;
			return *this;
		}
		float operator * (const vec::Vector2 vector) {
			return (float)((x * vector.x) + (y * vector.y));
		}
		vec::Vector2 operator * (const float scalar) {
			vec::Vector2 result = *this;
			result.x *= scalar;
			result.y *= scalar;
			return result;
		}
		vec::Vector2 operator * (const int scalar) {
			vec::Vector2 result = *this;
			result.x *= scalar;
			result.y *= scalar;
			return result;
		}
		vec::Vector2 operator *= (const float scalar) {
			x *= scalar;
			y *= scalar;
			return *this;
		}
		vec::Vector2 operator *= (const int scalar) {
			x *= scalar;
			y *= scalar;
			return *this;
		}
		vec::Vector2 operator = (const vec::Vector2 vector) {
			this->x = vector.x;
			this->y = vector.y;
			return *this;
		}
		vec::Vector2 operator = (const float value) {
			this->x = value;
			this->y = value;
			return *this;
		}
		vec::Vector2 operator = (const int value) {
			this->x = static_cast<float>(value);
			this->y = static_cast<float>(value);
			return *this;
		}
		bool operator == (const vec::Vector2 vector) {
			if (x == vector.x && y == vector.y) {
				return true;
			}
			else {
				return false;
			}
		}
		bool operator == (const float value) {
			if (x == value && y == value) {
				return true;
			}
			else {
				return false;
			}
		}
		bool operator == (const int value) {
			if (x == value && y == value) {
				return true;
			}
			else {
				return false;
			}
		}
		bool operator != (const vec::Vector2 vector) {
			if (x == vector.x && y == vector.y) {
				return false;
			}
			else {
				return true;
			}
		}
		bool operator != (const float value) {
			if (x == value && y == value) {
				return false;
			}
			else {
				return true;
			}
		}
		bool operator != (const int value) {
			if (x == value && y == value) {
				return false;
			}
			else {
				return true;
			}
		}
	};
	std::ostream& operator <<(std::ostream& os, const vec::Vector2& v);
}

namespace con {

	void UpdateConInfo();

	void SetColor(int foreground, int background);
	bool GoToxy(int x = 0, int y = 0);
	bool GoToxy(float x = 0.0f, float y = 0.0f);
	bool GoToxy(vec::Vector2 v);
	int GetCursorLine();
	void Clear();

	void DrawVector(vec::Vector2 v, vec::Vector2 pos, int borderSize);
	void ClearVector(vec::Vector2 v, vec::Vector2 pos, int borderSize);
	void DrawBorder(vec::Vector2 pos, float borderSize);
}

namespace rend {

	//colors

#define GOLD_B       { 255, 203, 0, 255 }
#define ORANGE_B     { 255, 161, 0, 255 }
#define PINK_B       { 255, 109, 194, 255 }
#define MAROON_B     { 190, 33, 55, 255 }
#define DARKGREEN_B  { 0, 117, 44, 255 }
#define SKYBLUE_B    { 102, 191, 255, 255 }
#define DARKBLUE_B   { 0, 82, 172, 255 }
#define PURPLE_B     { 200, 122, 255, 255 }
#define VIOLET_B     { 135, 60, 190, 255 }
#define DARKPURPLE_B { 112, 31, 126, 255 }
#define BEIGE_B      { 211, 176, 131, 255 }
#define BROWN_B      { 127, 106, 79, 255 }
#define DARKBROWN_B  { 76, 63, 47, 255 }

#define TRANSPARENT_B	{ 0, 0, 0, 0 }
#define SEMITRANSPARENT_B	{ 0, 0, 0, 128 }

#define WHITE_B	{ 255, 255, 255, 255 }
#define LIGHTGREY_B	{ 192, 192, 192, 255 }
#define GREY_B	{ 128, 128, 128, 255 }
#define DARKGREY_B	{ 64, 64, 64, 255 }
#define BLACK_B	{ 0, 0, 0, 255 }

#define RED_B		{ 255, 0, 0, 255 }
#define GREEN_B	{ 0, 255, 0, 255 }
#define BLUE_B	{ 0, 0, 255, 255 }

#define YELLOW_B		{ 255, 255, 0, 255 }
#define LIGHTBLUE_B		{ 0, 255, 255, 255 }
#define MAGENTA_B		{ 255, 0, 255, 255 }

#define LIME_B		{ 128, 255, 0, 255 }
#define FUSCHIA_B		{ 255, 0, 128, 255 }
#define MINT_B		{ 0, 255, 128, 255 }
#define BLUEBERRY_B		{ 0, 128, 255, 255 }

//resolutions

	enum class WindowType {
		WINDOWED,
		FULLSCREEN
	};

	enum class GraphicsLib {
		NONE,
		RAYLIB,
		SIGIL
	};

	enum class InfoMode {
		NONE,
		BASIC,
		ADVANCED
	};

	bool ChangeRenderer();

	void OpenWindow(vec::Vector2 size, const char* title, bool fullscreen = false);
	bool ShouldExit();
	void Close();

	vec::Vector2 GetWindowSize();
	vec::Vector2 GetMousePos();
	float GetDeltaTime();
	void UpdateDeltaTime();
	void UpdateFramerate();

	extern rend::GraphicsLib activeGraphics;
	extern rend::GraphicsLib nextGraphics;

	extern ctrl::Key frameInfoKey;
	extern InfoMode frameInfo;
	extern ctrl::Key devInfoKey;
	extern InfoMode devInfo;
	extern ctrl::Key devModeKey;
	extern InfoMode devMode;

	extern int frameCounter;
	extern int framerate;
	extern float secondCounter;
	extern float secondSmallCounter;
	extern int secondFrameCounter;
	extern float deltaTime;

	extern float fullscreen;

	extern vec::Vector2 windowSize;
	extern float windowRatio;

	extern std::string mouseSprite;
	extern vec::Vector2 mousePos;
	extern vec::Vector2 lastMousePos;
	extern vec::Vector2 mouseDelta;

	extern bool pixelMode;
}

namespace coll {

	bool PointOnRec(vec::Vector2 point, vec::Vector2 pos, vec::Vector2 size);
	bool PointOnCircle(vec::Vector2 point, vec::Vector2 pos, float radius);
	bool RecOnRec(vec::Vector2 pos1, vec::Vector2 size1, vec::Vector2 pos2, vec::Vector2 size2, vec::Vector2& point);
	bool CircleOnCircle(vec::Vector2 pos1, vec::Vector2 pos2, float radius1, float radius2, vec::Vector2& point);
	bool CircleOnCircle(vec::Vector2 pos1, vec::Vector2 pos2, float radius1, float radius2);
	//bool RecOnElipse(vec::Vector2 pos1, vec::Vector2 size1, vec::Vector2 pos2, vec::Vector2 size2, vec::Vector2& point);
	//bool RecOnElipse(vec::Vector2 pos1, vec::Vector2 size1, vec::Vector2 pos2, vec::Vector2 size2, vec::Vector2& point);
	//bool RecOnCircle(vec::Vector2 pos1, vec::Vector2 size1, vec::Vector2 pos2, vec::Vector2 size2, vec::Vector2& point);
	//bool RecOnCircle(vec::Vector2 pos1, vec::Vector2 size1, vec::Vector2 pos2, float radius, vec::Vector2& point);
}

namespace drw {

	struct SpriteData {

		bool active = false;
		int id = 0;

		std::string name = "Sprite";
		std::string file = "res/sprites/NoTexture.png";

		vec::Vector2 size = { 0,0 };
		vec::Vector2 offset = { 0,0 };
	};

	extern SpriteData defaultSprite;

	const int spriteDataMaxAmount = 100;
	extern SpriteData spriteDataList[spriteDataMaxAmount];

	struct AnimationData {

		bool active = false;
		int id = 0;

		int frames = 1;

		float timeOffset = 0.0f;
		float duration = 1.0f;
	};

	struct FontData {

		bool active = false;
		int id = 0;

		std::string name = "No Font";
		std::string file = "res/fonts/ReemKufiFunRegular.ttf";

		float spacing = 1.0f;
		float size = 50;

		Font font = {};
	};

	extern FontData defaultFont;

	const int fontDataMaxAmount = 10;
	extern FontData fontDataList[fontDataMaxAmount];

	struct TextData {

		int fontID = 0;

		std::string text = "text";
		int fontSize = 50;

		vec::Vector2 size = { 0,0 };
		vec::Vector2 origin = { 0,0 };
		float rotation = 0;
	};

	vec::Vector2 PercentToScreen(vec::Vector2 vec);

	void Clear(bColor color = BLACK_B);

	int InitSpriteData(SpriteData& spriteData);
	bool DeInitSpriteData(SpriteData& spriteData);

	int InitAnimData(AnimationData& animData, SpriteData spriteData[], int frames);

	int InitFontData(FontData& fontData);

	void Begin();
	void End();

	bool Animation(AnimationData& animation, vec::Vector2 pos = { 0,0 }, vec::Vector2 size = { 0,0 }, vec::Vector2 offset = { 0,0 }, bColor color = WHITE_B);
	bool Sprite(SpriteData sprite, vec::Vector2 pos = { 0,0 }, vec::Vector2 size = { 0,0 }, vec::Vector2 offset = { 0,0 }, bColor color = WHITE_B);
	void Text(const char* text, TextData& textData, vec::Vector2 pos = { 0,0 }, int fontSize = 10, vec::Vector2 offset = { 0,0 }, bColor color = WHITE_B);
	void Text(const char* text, vec::Vector2 pos = { 0,0 }, int fontSize = 10, vec::Vector2 offset = { 0,0 }, bColor color = WHITE_B);
	void Text(TextData& textData, vec::Vector2 pos = { 0,0 }, int fontSize = 10, vec::Vector2 offset = { 0,0 }, bColor color = WHITE_B);
	void Triangle(vec::Vector2 p1, vec::Vector2 p2, vec::Vector2 p3, bColor color = WHITE_B);
	void Rectangle(vec::Vector2 pos, vec::Vector2 size, bColor color = WHITE_B, vec::Vector2 offset = { 0,0 });
	void Circle(vec::Vector2 pos, vec::Vector2 size, bColor color = WHITE_B);
	void Line(vec::Vector2 start, vec::Vector2 end, bColor color = WHITE_B);
	void Line(vec::Vector2 start, vec::Vector2 end,float size = 0.0f, bColor color = WHITE_B);
}

namespace snd {

	struct AudioData {

		bool active = false;
		int id = 0;

		std::string name = "No Audio";
		std::string file = "res/audio/ButtonClick.wav";

		Sound sound = {};
	};

	extern AudioData defaultAudio;

	const int audioDataMaxAmount = 100;
	extern AudioData audioDataList[audioDataMaxAmount];

	void StartAudioDevice();
	void EndAudioDevice();

	int InitAudioData(AudioData& audioData);
	void DeInit(AudioData& audioData);
	void Play(int id);
	void Pause(int id, bool pause = true);
	void Stop(int id);

	bool IsPlaying(int id);
}

namespace btn {

	struct Container {

		vec::Vector2 pos;
		vec::Vector2 offset;

		vec::Vector2 size;

		bool isRendered = false;
		bool useTexture = false;

		bColor color = WHITE_B;

		int textureID = 0;
	};

	struct Button {

		// active
		bool isActive = false;
		bool isRendered = true;

		//signal

		bool signal = false;

		bool startSignalTimer = false;
		float signalTimer = 0.0f;
		float signalTimerLimit = 0.2f;

		// interface

		bool isClicked = false;
		bool isHovered = false;

		bool isClickedDownChange = false;
		bool isClickedUpChange = false;
		bool isHoveredChange = false;

		//positions
		vec::Vector2 pos = { 0,0 };
		vec::Vector2 offset = { 0,0 };

		vec::Vector2 clickedOffset = { 0,-0.25f };

		vec::Vector2 size = { 0,0 };

		//Text
		bool useText = true;

		drw::TextData textData;

		//colors
		bColor activeColor = LIGHTGREY_B;

		bColor mainColor = LIGHTGREY_B;
		bColor hoveredColor = DARKGREY_B;

		bColor textColor = GREEN_B;

		//textures
		bool useSprite = false;

		int activeTextureID = 0;

		int mainTextureID = 0;
		int hoveredTextureID = 0;

		//sounds

		int clickedDownSoundID = 0;
		int clickedUpSoundID = 0;
		int hoveredSoundID = 0;
	};

	void InitButtonDefaults();
	void DeInitButtonDefaults();

	void Init(Button& button);
	void Init(Button buttons[], int buttonsSize);
	void Init(Button& button, Container& container, bool firstInit = false);
	void Init(Container& container, Button buttons[], int buttonsSize);
	void Init(Container& container);

	bool isHovered(Button button);
	bool isClicked(Button button);

	void UpdateInput(Button& button);
	void UpdateInput(Button buttons[], int buttonsSize);

	void Sound(Button button);
	void Sound(Button buttons[], int buttonsSize);
	void Draw(Button& button);
	void Draw(Container container);
	void Draw(Button buttons[], int buttonsSize);
	void Draw(Container container, Button buttons[], int buttonsSize);
}

namespace prtcl {

	struct ParticleActivator {

		bool loop = false;

		bool active = false;
		int id = 0;

		bool animated = false;
		
		float startingPosInfluence = 0.0f;

		drw::AnimationData animation;

		vec::Vector2 pos = { 0,0 };
		vec::Vector2 startingOffset = { 0,0 };

		vec::Vector2 minSize = { 0.01f,0.01f };
		vec::Vector2 maxSize = { 0.05f,0.05f };

		vec::Vector2 delay = { 0.0f,0.5f };
		vec::Vector2 lifetime = { 0.5f,1.5f };
		int amount = 3;

		vec::Vector2 direction = { 0,1 };
		vec::Vector2 rotation = { -180,180 };
		vec::Vector2 speed = { 0.5f,1.0f };

		float spread = 45;
	};

	struct ParticleData {

		bool active = false;
		bool alive = false;

		int id = 0;

		bool animated = false;

		drw::AnimationData animation;

		int amount = 1;

		float rotationChange = 0;
		vec::Vector2 rotation = { 0,0 };

		vec::Vector2 pos = { 0,0 };
		vec::Vector2 size = { 0,0 };
		vec::Vector2 direction = { 0,1 };
		float speed = 1;

		float lifetime = 1.0f;
		float delay = 0.0f;
	};

	void Init(ParticleActivator& activator, ParticleData particles[]);
	void Init(ParticleActivator& activator, ParticleData& particle);

	bool Update(ParticleData& particles);
	void Update(ParticleActivator& activator, ParticleData particles[]);
	void Draw(ParticleData particles[]);
}

namespace upd {

	bool isMouseOver(vec::Vector2 pos, vec::Vector2 size);
	bool isMouseClicking(vec::Vector2 pos, vec::Vector2 size);
}

namespace bLib {

	void Init(std::string windowName = "Main");
	void UpdateStart();
	void UpdateEnd();
}