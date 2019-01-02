#pragma once

#include <glm/glm.hpp>

#include <string>

struct GLFWwindow;

namespace agf {

	class Window
	{
	public:
		Window(unsigned width, unsigned height, const std::string& title);
		~Window();

		void SwapBuffers() const;
		void PollEvents() const;

		bool IsMouseButtonPressed(unsigned button) const;
		bool IsKeyPressed(unsigned key) const;

		glm::vec2 GetCusorPosition() const;
		bool ShouldClose() const;

		inline unsigned GetWidth() const { return m_Width; }
		inline unsigned GetHeight() const { return m_Height; }

	private:
		unsigned m_Width;
		unsigned m_Height;
		std::string m_Title;
		GLFWwindow* m_GLFWwindow;
	};

}

#define AGF_RELEASE                0
#define AGF_PRESS                  1
#define AGF_REPEAT                 2

#define AGF_KEY_SPACE              32
#define AGF_KEY_APOSTROPHE         39  /* ' */
#define AGF_KEY_COMMA              44  /* , */
#define AGF_KEY_MINUS              45  /* - */
#define AGF_KEY_PERIOD             46  /* . */
#define AGF_KEY_SLASH              47  /* / */
#define AGF_KEY_0                  48
#define AGF_KEY_1                  49
#define AGF_KEY_2                  50
#define AGF_KEY_3                  51
#define AGF_KEY_4                  52
#define AGF_KEY_5                  53
#define AGF_KEY_6                  54
#define AGF_KEY_7                  55
#define AGF_KEY_8                  56
#define AGF_KEY_9                  57
#define AGF_KEY_SEMICOLON          59  /* ; */
#define AGF_KEY_EQUAL              61  /* = */
#define AGF_KEY_A                  65
#define AGF_KEY_B                  66
#define AGF_KEY_C                  67
#define AGF_KEY_D                  68
#define AGF_KEY_E                  69
#define AGF_KEY_F                  70
#define AGF_KEY_G                  71
#define AGF_KEY_H                  72
#define AGF_KEY_I                  73
#define AGF_KEY_J                  74
#define AGF_KEY_K                  75
#define AGF_KEY_L                  76
#define AGF_KEY_M                  77
#define AGF_KEY_N                  78
#define AGF_KEY_O                  79
#define AGF_KEY_P                  80
#define AGF_KEY_Q                  81
#define AGF_KEY_R                  82
#define AGF_KEY_S                  83
#define AGF_KEY_T                  84
#define AGF_KEY_U                  85
#define AGF_KEY_V                  86
#define AGF_KEY_W                  87
#define AGF_KEY_X                  88
#define AGF_KEY_Y                  89
#define AGF_KEY_Z                  90
#define AGF_KEY_LEFT_BRACKET       91  /* [ */
#define AGF_KEY_BACKSLASH          92  /* \ */
#define AGF_KEY_RIGHT_BRACKET      93  /* ] */
#define AGF_KEY_GRAVE_ACCENT       96  /* ` */
#define AGF_KEY_WORLD_1            161 /* non-US #1 */
#define AGF_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define AGF_KEY_ESCAPE             256
#define AGF_KEY_ENTER              257
#define AGF_KEY_TAB                258
#define AGF_KEY_BACKSPACE          259
#define AGF_KEY_INSERT             260
#define AGF_KEY_DELETE             261
#define AGF_KEY_RIGHT              262
#define AGF_KEY_LEFT               263
#define AGF_KEY_DOWN               264
#define AGF_KEY_UP                 265
#define AGF_KEY_PAGE_UP            266
#define AGF_KEY_PAGE_DOWN          267
#define AGF_KEY_HOME               268
#define AGF_KEY_END                269
#define AGF_KEY_CAPS_LOCK          280
#define AGF_KEY_SCROLL_LOCK        281
#define AGF_KEY_NUM_LOCK           282
#define AGF_KEY_PRINT_SCREEN       283
#define AGF_KEY_PAUSE              284
#define AGF_KEY_F1                 290
#define AGF_KEY_F2                 291
#define AGF_KEY_F3                 292
#define AGF_KEY_F4                 293
#define AGF_KEY_F5                 294
#define AGF_KEY_F6                 295
#define AGF_KEY_F7                 296
#define AGF_KEY_F8                 297
#define AGF_KEY_F9                 298
#define AGF_KEY_F10                299
#define AGF_KEY_F11                300
#define AGF_KEY_F12                301
#define AGF_KEY_F13                302
#define AGF_KEY_F14                303
#define AGF_KEY_F15                304
#define AGF_KEY_F16                305
#define AGF_KEY_F17                306
#define AGF_KEY_F18                307
#define AGF_KEY_F19                308
#define AGF_KEY_F20                309
#define AGF_KEY_F21                310
#define AGF_KEY_F22                311
#define AGF_KEY_F23                312
#define AGF_KEY_F24                313
#define AGF_KEY_F25                314
#define AGF_KEY_KP_0               320
#define AGF_KEY_KP_1               321
#define AGF_KEY_KP_2               322
#define AGF_KEY_KP_3               323
#define AGF_KEY_KP_4               324
#define AGF_KEY_KP_5               325
#define AGF_KEY_KP_6               326
#define AGF_KEY_KP_7               327
#define AGF_KEY_KP_8               328
#define AGF_KEY_KP_9               329
#define AGF_KEY_KP_DECIMAL         330
#define AGF_KEY_KP_DIVIDE          331
#define AGF_KEY_KP_MULTIPLY        332
#define AGF_KEY_KP_SUBTRACT        333
#define AGF_KEY_KP_ADD             334
#define AGF_KEY_KP_ENTER           335
#define AGF_KEY_KP_EQUAL           336
#define AGF_KEY_LEFT_SHIFT         340
#define AGF_KEY_LEFT_CONTROL       341
#define AGF_KEY_LEFT_ALT           342
#define AGF_KEY_LEFT_SUPER         343
#define AGF_KEY_RIGHT_SHIFT        344
#define AGF_KEY_RIGHT_CONTROL      345
#define AGF_KEY_RIGHT_ALT          346
#define AGF_KEY_RIGHT_SUPER        347
#define AGF_KEY_MENU               348

#define AGF_KEY_LAST               AGF_KEY_MENU

#define AGF_MOUSE_BUTTON_1         0
#define AGF_MOUSE_BUTTON_2         1
#define AGF_MOUSE_BUTTON_3         2
#define AGF_MOUSE_BUTTON_4         3
#define AGF_MOUSE_BUTTON_5         4
#define AGF_MOUSE_BUTTON_6         5
#define AGF_MOUSE_BUTTON_7         6
#define AGF_MOUSE_BUTTON_8         7
#define AGF_MOUSE_BUTTON_LAST      AGF_MOUSE_BUTTON_8
#define AGF_MOUSE_BUTTON_LEFT      AGF_MOUSE_BUTTON_1
#define AGF_MOUSE_BUTTON_RIGHT     AGF_MOUSE_BUTTON_2
#define AGF_MOUSE_BUTTON_MIDDLE    AGF_MOUSE_BUTTON_3