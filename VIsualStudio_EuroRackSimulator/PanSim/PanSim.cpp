
#include "imgui.h"
#include "../TestFrameImgUI/imgui_impl_sdl_gl3.h"
#include <stdio.h>
#include <GL/gl3w.h>    
#include <SDL.h>

#include "../libs/lodepng-master/lodepng.h"

#include  "PanHeader.h"


SDL_Surface* load_PNG(const char* filename)
{
	std::vector<unsigned char> buffer, image;
	lodepng::load_file(buffer, filename); //load the image file with given filename
	unsigned w, h;
	unsigned error = lodepng::decode(image, w, h, buffer); //decode the png

	if (error)
	{
		//std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		return 0;
	}

	Uint32 rmask, gmask, bmask, amask;
	/* SDL interprets each pixel as a 32-bit number, so our masks must depend
	on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	//avoid too large window size by downscaling large image
	unsigned jump = 1;
	//if(w / 1024 >= jump) jump = w / 1024 + 1;
	//if(h / 1024 >= jump) jump = h / 1024 + 1;

	SDL_Surface *dest = SDL_CreateRGBSurface(0, w, h, 32,
		rmask, gmask, bmask, amask);
	if (dest == NULL) {
		fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
		return 0;
	}

	if (SDL_MUSTLOCK(dest)) { SDL_LockSurface(dest); }

	//plot the pixels of the PNG file
	for (unsigned y = 0; y + jump - 1 < h; y += jump)
		for (unsigned x = 0; x + jump - 1 < w; x += jump)
		{
			//get RGBA components
			Uint32 r = image[4 * y * w + 4 * x + 0]; //red
			Uint32 g = image[4 * y * w + 4 * x + 1]; //green
			Uint32 b = image[4 * y * w + 4 * x + 2]; //blue
			Uint32 a = image[4 * y * w + 4 * x + 3]; //alpha

													 //make translucency visible by placing checkerboard pattern behind image
													 //int checkerColor = 191 + 64 * (((x / 16) % 2) == ((y / 16) % 2));
													 //r = (a * r + (255 - a) * checkerColor) / 255;
													 //g = (a * g + (255 - a) * checkerColor) / 255;
													 //b = (a * b + (255 - a) * checkerColor) / 255;

													 //give the color value to the pixel of the screenbuffer
			Uint32* bufp = (Uint32 *)dest->pixels + (y * dest->pitch / 4) / jump + (x / jump);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			*bufp = 0x01000000 * r + 65536 * g + 256 * b + a;
#else
			*bufp = 0x01000000 * a + 65536 * b + 256 * g + r;
#endif
		}

	if (SDL_MUSTLOCK(dest)) { SDL_UnlockSurface(dest); }
	return dest;
}

void SetupIcon(SDL_Window *window)
{

	SDL_SetWindowIcon(window, load_PNG("favicon-32x32.png"));

}

void PrintKnobs()
{
	
}

static bool MyKnob(const char* label, float* p_value, float v_min, float v_max)
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();

	float radius_outer = 20.0f;
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 center = ImVec2(pos.x + radius_outer, pos.y + radius_outer);
	float line_height = ImGui::GetTextLineHeight();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	float ANGLE_MIN = 3.141592f * 0.75f;
	float ANGLE_MAX = 3.141592f * 2.25f;

	ImGui::InvisibleButton(label, ImVec2(radius_outer * 2, radius_outer * 2 + line_height + style.ItemInnerSpacing.y));
	bool value_changed = false;
	bool is_active = ImGui::IsItemActive();
	bool is_hovered = ImGui::IsItemActive();
	if (is_active && io.MouseDelta.x != 0.0f)
	{
		float step = (v_max - v_min) / 200.0f;
		*p_value += io.MouseDelta.x * step;
		if (*p_value < v_min) *p_value = v_min;
		if (*p_value > v_max) *p_value = v_max;
		value_changed = true;
	}

	float t = (*p_value - v_min) / (v_max - v_min);
	float angle = ANGLE_MIN + (ANGLE_MAX - ANGLE_MIN) * t;
	float angle_cos = cosf(angle), angle_sin = sinf(angle);
	float radius_inner = radius_outer*0.40f;
	draw_list->AddCircleFilled(center, radius_outer, ImGui::GetColorU32(ImGuiCol_FrameBg), 16);
	draw_list->AddLine(ImVec2(center.x + angle_cos*radius_inner, center.y + angle_sin*radius_inner), ImVec2(center.x + angle_cos*(radius_outer - 2), center.y + angle_sin*(radius_outer - 2)), ImGui::GetColorU32(ImGuiCol_SliderGrabActive), 2.0f);
	draw_list->AddCircleFilled(center, radius_inner, ImGui::GetColorU32(is_active ? ImGuiCol_FrameBgActive : is_hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), 16);
	auto R = ImGui::CalcTextSize(label);
	draw_list->AddText(ImVec2(center.x - R.x/2, pos.y - line_height - style.ItemInnerSpacing.y), ImGui::GetColorU32(ImGuiCol_Text), label);

	if (is_active || is_hovered)
	{
		ImGui::SetNextWindowPos(ImVec2(pos.x - style.WindowPadding.x, pos.y - line_height - style.ItemInnerSpacing.y - style.WindowPadding.y));
		ImGui::BeginTooltip();
		ImGui::Text("%.3f", *p_value);
		ImGui::EndTooltip();
	}

	return value_changed;
}

bool LedButton(const char* label, bool* v)
{
	
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();

	float radius_outer = 10.0f;
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 center = ImVec2(pos.x + radius_outer, pos.y + radius_outer);
	float line_height = ImGui::GetTextLineHeight();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();


	bool pressed = ImGui::InvisibleButton(label, ImVec2(radius_outer * 2, radius_outer * 2 + line_height + style.ItemInnerSpacing.y));
	bool is_active = ImGui::IsItemActive();
	bool is_hovered = ImGui::IsItemHovered();


	draw_list->AddCircleFilled(center, radius_outer, ImGui::GetColorU32(is_active ? ImGuiCol_FrameBgActive : is_hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), 16);
	auto R = ImGui::CalcTextSize(label);
	draw_list->AddText(ImVec2(center.x - R.x / 2, pos.y - line_height - style.ItemInnerSpacing.y), ImGui::GetColorU32(ImGuiCol_Text), label);

	//if (pressed) *v = !*v;
	if (*v)
	{
		draw_list->AddCircleFilled(center, radius_outer,IM_COL32(255,255,0,255) , 16);

	}

	return pressed;
}

bool ImLed(const char* label, bool* v)
{

	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();

	float radius_outer = 5.0f;
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 center = ImVec2(pos.x + radius_outer, pos.y + radius_outer);
	float line_height = ImGui::GetTextLineHeight();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();


	bool is_active = ImGui::IsItemActive();
	bool is_hovered = ImGui::IsItemHovered();


	draw_list->AddCircleFilled(center, radius_outer, IM_COL32(0,0,0,255), 16);
	auto R = ImGui::CalcTextSize(label);
	draw_list->AddText(ImVec2(center.x - R.x / 2, pos.y - line_height - style.ItemInnerSpacing.y), ImGui::GetColorU32(ImGuiCol_Text), label);

	
	if (*v)
	{
		draw_list->AddCircleFilled(center, radius_outer, IM_COL32(255, 255, 0, 255), 16);

	}

	return false;
}

int main(int, char**)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}

	// Setup window
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);
	SDL_Window *window = SDL_CreateWindow("Synton PAN Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 2000, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SetupIcon(window);
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(1); // Enable vsync
	gl3wInit();

	// Setup ImGui binding
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	ImGui_ImplSdlGL3_Init(window);

	// Setup style
	ImGui::StyleColorsLight();
	
	ImVec4 clear_color = ImVec4(1.0f / 255.0f, 58.0f / 255.0f, 66.0f / 255.0f, 1.00f);

	// racing turqoise: 1,58,66
	// haringmuts geel: 255, 234,0 

#define RACINGGREEN IM_COL32(1, 58, 66, 255)

	bool done = false;
	int32_t clkcolor = ImColor::HSV(0, 1, 1);
	int32_t tickcolor = ImColor::HSV(0.25, 1, 1);;// IM_COL32(0, 128, 255, 255);
	int32_t beatcolor = ImColor::HSV(0.5, 1, 1);
	int32_t loopcolor = ImColor::HSV(0.75, 1, 1);

	int32_t clklinecolor = ImColor::HSV(0, 1, 1,0.5);
	int32_t ticklinecolor = ImColor::HSV(0.25, 1, 1, 0.5);;// IM_COL32(0, 128, 255, 255);
	int32_t beatlinecolor = ImColor::HSV(0.5, 1, 1, 0.5);
	int32_t looplinecolor = ImColor::HSV(0.75, 1, 1, 0.5);


	int32_t gatecolor = ImColor::HSV(0, 0, 0.3);
	int32_t accentcolor = ImColor::HSV(0, 0, 1);

	ImFont* pFont = io.Fonts->AddFontFromFileTTF("Fontfabric - Panton.otf", 11.0f);
	ImFont* pFontBold = io.Fonts->AddFontFromFileTTF("Fontfabric - Panton ExtraBold.otf", 18.0f);

	unsigned char * pixels;
	int width, height, bytes_per_pixels;

	
	ImGui::GetStyle().ItemInnerSpacing = ImVec2(5, 5);;
	ImGui::GetStyle().ItemSpacing = ImVec2(5, 5);
	ImGui::GetStyle().FramePadding = ImVec2(5, 5);
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 1.0f, 1.0f, .800f);

	static bool parameters = true;


	while (!done)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSdlGL3_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				done = true;
		}
		ImGui_ImplSdlGL3_NewFrame(window);

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("PanSim Windows"))
			{
				ImGui::MenuItem("Pan Parameters", NULL, &parameters);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if (parameters)
		{
			ImGui::PushFont(pFontBold);
			{
				ImGui::Begin("Pan Parameters", &parameters, ImGuiWindowFlags_AlwaysAutoResize);
				ImGui::PushFont(pFont);

				ImVec2 pos = ImGui::GetCursorScreenPos();
				float xscalefac = 60;
				float yscalefac = 80;
				for (int i = 0; i < __KNOB_COUNT; i++)
				{

					ImGui::SetCursorScreenPos(ImVec2(pos.x + Knobs[i].x * xscalefac, pos.y + Knobs[i].y * yscalefac));
					if (MyKnob(Knobs[i].name, &Knobs[i].value, 0, 1))
					{
						KnobChanged(Knobs[i].id, Knobs[i].value);
					}
				}

				for (int i = 0; i < __LEDBUTTON_COUNT; i++)
				{
					ImGui::SetCursorScreenPos(ImVec2(pos.x + Buttons[i].x * xscalefac, pos.y + Buttons[i].y * yscalefac));
					if (LedButton(Buttons[i].name, &Buttons[i].value))
					{
						ButtonPressed(Buttons[i].id, Buttons[i].value);

					}
				}
				for (int i = 0; i < __LED_COUNT; i++)
				{
					ImGui::SetCursorScreenPos(ImVec2(pos.x + Leds[i].x * xscalefac, pos.y + Leds[i].y * yscalefac));
					ImLed(Leds[i].name, &Leds[i].value);
				}

				ImGui::SetCursorScreenPos(ImVec2(pos.x + TheScreen.x * xscalefac, pos.y + TheScreen.y * yscalefac));
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 255));

				ImGui::BeginChild("screen", ImVec2(xscalefac * TheScreen.width, yscalefac * TheScreen.height), true);
				RenderScreen();
				ImGui::EndChild();
				ImGui::PopStyleColor();
				ImGui::PopFont();


				ImGui::End();
				ImGui::PopFont();
			}
		}

	

		glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();
		ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
		//ImGui::PopFont();
		SDL_GL_SwapWindow(window);
	}

	ImGui_ImplSdlGL3_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
