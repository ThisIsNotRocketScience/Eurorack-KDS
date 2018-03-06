#include "../../Tuesday/Sources/Tuesday.h"

Tuesday_PatternGen Tuesday;
Tuesday_Settings TuesdaySettings;
Tuesday_Params TuesdayParams;
Tuesday_RandomGen MainRandom;
long oldseed = -1;

extern "C"
{
	void __cdecl doTick()
	{
		Tuesday_Tick(&Tuesday, &TuesdayParams);
	}
	void __cdecl DoClock(int state)
	{
		Tuesday_Clock(&Tuesday, &TuesdaySettings, &TuesdayParams, state);
	}
}


// ImGui - standalone example application for SDL2 + OpenGL
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)
// (GL3W is a helper library to access OpenGL functions since there is no standard header to access modern OpenGL functions easily. Alternatives are GLEW, Glad, etc.)

#include "imgui.h"
#include "../TestFrameImgUI/imgui_impl_sdl_gl3.h"
#include <stdio.h>
#include <GL/gl3w.h>    // This example is using gl3w to access OpenGL functions (because it is small). You may use glew/glad/glLoadGen/etc. whatever already works for you.
#include <SDL.h>

#include "../../WobblerV2/Sources/Wobbler2.h"

#include "../libs/lodepng-master/lodepng.h"


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

enum
{
	ADC_TEMPO,
	ADC_X,
	ADC_Y,
	ADC_BANG,
	ADC_COUNT
};
int cursor = 0;
ImVec2 Points[1000];
ImVec2 EnvPoints[1000];
ImVec2 Points2[1000];

typedef struct res_Struct
{
	bool clkin, beatled, tickled, clkout, loopled, acc, gate;
	int leds[16];
	int notecv;
	int velcv;
	
}
res_Struct;

res_Struct res[10000];

int main(int, char**)
{
	Tuesday_Init(&Tuesday);

	Tuesday_LoadSettings(&TuesdaySettings, &TuesdayParams);
	Tuesday_RandomSeed(&MainRandom, oldseed);
	EuroRack_InitCalibration();

	

	Tuesday_SetupClockSubdivision(&Tuesday, &TuesdaySettings, &TuesdayParams);
	Tuesday_LoadDefaults(&TuesdaySettings, &TuesdayParams);


	Tuesday_Generate(&Tuesday, &TuesdayParams, &TuesdaySettings);
	// Setup SDL
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
	SDL_Window *window = SDL_CreateWindow("TiNRS Tuesday Inspection GUI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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
	static int counter = 0;
	static int updaterate = 1;
	static int T = 0;
	static bool externalclock = true;
	static int clocktiming = 10;
	float fX = 0, fY = 0, fBang = 1, fTempo = 0;
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

	ImFont* pFont = io.Fonts->AddFontFromFileTTF("Fontfabric - Panton.otf", 14.0f);
	ImFont* pFontBold = io.Fonts->AddFontFromFileTTF("Fontfabric - Panton ExtraBold.otf", 18.0f);

	unsigned char * pixels;
	int width, height, bytes_per_pixels;

	
	ImGui::GetStyle().ItemInnerSpacing = ImVec2(5, 5);;
	ImGui::GetStyle().ItemSpacing = ImVec2(5, 5);
	ImGui::GetStyle().FramePadding = ImVec2(5, 5);
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 1.0f, 1.0f, .800f);
	static bool parameters = true;
	static bool waveoutputs = true;


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

			if (ImGui::BeginMenu("Tuesday Windows"))
			{
				ImGui::MenuItem("Output Waveforms", NULL, &waveoutputs);
				ImGui::MenuItem("Tuesday Parameters", NULL, &parameters);

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if (parameters)
		{
			ImGui::PushFont(pFontBold);
			{
				ImGui::Begin("Tuesday Parameters", &parameters, ImGuiWindowFlags_AlwaysAutoResize);
				ImGui::PushFont(pFont);


				{



					ImGui::Checkbox("external clock", &externalclock);
					if (externalclock)
					{
						ImGui::SliderInt("clockspacing", &clocktiming, 1, 20);


						static int subdiv = 3;
						static int lastsubdiv = -1;
						ImGui::Text("subdiv mode:"); ImGui::SameLine();
						ImGui::RadioButton("1:1", &subdiv, 0); ImGui::SameLine();
						ImGui::RadioButton("1:2", &subdiv, 1); ImGui::SameLine();
						ImGui::RadioButton("16ppqn", &subdiv, 2); ImGui::SameLine();
						ImGui::RadioButton("24ppqn", &subdiv, 3);
						if (subdiv != lastsubdiv)
						{
							TuesdaySettings.ClockSubDivMode = subdiv;
							lastsubdiv = subdiv;
							Tuesday_SetupClockSubdivision(&Tuesday, &TuesdaySettings, &TuesdayParams);

						}


					}



					ImGui::SliderFloat("Tempo", &fTempo, 0.0f, 1.0f);
					ImGui::SliderFloat("X", &fX, 0.0f, 1.0f);
					ImGui::SliderFloat("Y", &fY, 0.0f, 1.0f);
					ImGui::SliderFloat("!", &fBang, 0.0f, 1.0f);


					static int algo = 3;
					ImGui::Text("Algo:"); ImGui::SameLine();
					ImGui::RadioButton("a1", &algo, 0); ImGui::SameLine();
					ImGui::RadioButton("a2", &algo, 1); ImGui::SameLine();
					ImGui::RadioButton("a3", &algo, 2); ImGui::SameLine();
					ImGui::RadioButton("a4", &algo, 3); ImGui::SameLine(); ImGui::Text("%d", (int)TuesdaySettings.algooptions[algo]);

					static int scale = 3;
					ImGui::Text("scale:"); ImGui::SameLine();
					ImGui::RadioButton("s1", &scale, 0); ImGui::SameLine();
					ImGui::RadioButton("s2", &scale, 1); ImGui::SameLine();
					ImGui::RadioButton("s3", &scale, 2); ImGui::SameLine();
					ImGui::RadioButton("s4", &scale, 3); ImGui::SameLine(); ImGui::Text("%d", (int)TuesdaySettings.scale[scale]);

					static int ticks = 2;
					ImGui::Text("ticks:"); ImGui::SameLine();
					ImGui::RadioButton("t1", &ticks, 0); ImGui::SameLine();
					ImGui::RadioButton("t2", &ticks, 1); ImGui::SameLine();
					ImGui::RadioButton("t3", &ticks, 2); ImGui::SameLine();
					ImGui::RadioButton("t4", &ticks, 3); ImGui::SameLine(); ImGui::Text("%d", (int)TuesdaySettings.tpboptions[ticks]);

					static int beats = 0;
					ImGui::Text("beats:"); ImGui::SameLine();
					ImGui::RadioButton("b1", &beats, 0); ImGui::SameLine();
					ImGui::RadioButton("b2", &beats, 1); ImGui::SameLine();
					ImGui::RadioButton("b3", &beats, 2); ImGui::SameLine();
					ImGui::RadioButton("b4", &beats, 3); ImGui::SameLine(); ImGui::Text("%d", (int)TuesdaySettings.beatoptions[beats]);

					TuesdayParams.algo = algo;
					TuesdayParams.beatopt = beats;
					TuesdayParams.scale = scale;
					TuesdayParams.tpbopt = ticks;
					static int lastopts = -1;
					if (algo + beats + scale + ticks != lastopts)
					{
						lastopts = algo + beats + scale + ticks;
						Tuesday.switchmode = 1;
					}
					ImGui::SliderInt("Update Speed", &updaterate, 0, 20);


				}
				ImGui::PopFont();
				ImGui::End();
				ImGui::PopFont();

			}
		}
		int minN = 100000;
		int maxN = -100000;
		int minP = 100000;
		int maxP = -100000;

		uint16_t adcchannels[ADC_COUNT];
		adcchannels[ADC_X] = (int)((1 - fX) * 0xffff);
		adcchannels[ADC_Y] = (int)((1 - fY) * 0xffff);
		adcchannels[ADC_TEMPO] = (int)((1 - fTempo) * 0xffff);
		adcchannels[ADC_BANG] = (int)((1 - fBang) * 0xffff);;

		Tuesday.seed1 = (adcchannels[ADC_X] >> 8);
		Tuesday.seed2 = (adcchannels[ADC_Y] >> 8);
		Tuesday.intensity = (adcchannels[ADC_BANG] >> 8);
		Tuesday.tempo = (adcchannels[ADC_TEMPO] >> 8);
		long newseed = (Tuesday.seed1) + (Tuesday.seed2 << 8);
		if (newseed != oldseed) Tuesday.switchmode = 1;

		if (Tuesday.switchmode == 1)
		{
			Tuesday.switchmode = 0;
			Tuesday_RandomSeed(&MainRandom, newseed);
			Tuesday_Generate(&Tuesday, &TuesdayParams, &TuesdaySettings);
			oldseed = newseed;
		}
		
		for (int i = 0; i < updaterate*10; i++)
		{
			T++;
			bool inclk = false;
			static bool lastclk = true;
			if (externalclock)
			{
				if (T%(clocktiming*5) == 0)
				{
					inclk = true;
				}
			}
			res[cursor].clkin = inclk;
			if (lastclk != inclk)
			{
				Tuesday_ExtClock(&Tuesday, &TuesdayParams, &TuesdaySettings, inclk ? 1 : 0);
				lastclk = inclk;
			}
			Tuesday_TimerTick(&Tuesday, &TuesdayParams);
			res[cursor].notecv = Tuesday.CVOut;
			res[cursor].gate = Tuesday.Gates[GATE_GATE];
			res[cursor].beatled = Tuesday.Gates[GATE_BEAT];
			res[cursor].tickled = Tuesday.Gates[GATE_TICK];
			res[cursor].clkout = Tuesday.Gates[GATE_CLOCK];
			res[cursor].acc = Tuesday.Gates[GATE_ACCENT];
			res[cursor].loopled = Tuesday.Gates[GATE_LOOP];

			cursor = (cursor + 1) % 10000;
		}
		ImVec2 p;
		if(waveoutputs){
			ImGui::PushFont(pFontBold);
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 255));
			ImGui::Begin("Wave outputs", &waveoutputs, ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255, 255, 200, 255));
			ImGui::BeginChild("Normal", ImVec2(1000, 240), true);
			p = ImGui::GetCursorScreenPos();

			for (int i = 0; i < 500; i++)
			{
				int idx = (cursor - 1 - i*10  + 10000) % 10000;
				int idxb = (cursor - 1 - (i-1) * 10 + 10000) % 10000;
				int N = res[idx].notecv/65536;
				if (N > maxN) maxN = N;
				if (N < minN) minN = N;

				Points[i].x = i*2 + p.x;
				Points[i].y = p.y + 100 - ((N - 2048) / 20);
				bool gate = false;
				bool acc = false;
				bool beat = false;
				bool tick = false;
				bool loop = false;
				bool clk = false;
				for (int j = 0; j < 1; j++) {
					if (res[idx + j].gate) gate = true;
					if (res[idx + j].tickled) tick= true;
					if (res[idx + j].acc) acc = true;
					if (res[idx + j].loopled) loop = true;
					if (res[idx + j].beatled) beat = true;
					if (res[idx + j].clkin) clk= true;
				};
			
				if (gate)
				{
					ImVec2 S(i * 2 + p.x, p.y + 100);
					ImGui::GetWindowDrawList()->AddCircleFilled(S,2,gatecolor,4);
				}
				if (acc)
				{
					ImVec2 S(i * 2 + p.x, p.y + 100);
					ImGui::GetWindowDrawList()->AddCircle(S, 3, accentcolor,4);
				}

				if (clk)
				{
					ImVec2 S(i * 2 + p.x, p.y + 0);
					ImGui::GetWindowDrawList()->AddCircleFilled(S, 2, clkcolor, 4);
					ImGui::GetWindowDrawList()->AddLine(S, ImVec2(S.x, p.y + 100), clklinecolor);

				}

				if (loop)
				{
					ImVec2 S(i * 2 + p.x, p.y + 20);
					ImGui::GetWindowDrawList()->AddCircleFilled(S, 12, loopcolor, 12);
					ImGui::GetWindowDrawList()->AddLine(S, ImVec2(S.x, p.y + 100), looplinecolor,5);

				}
				if (beat)
				{
					ImVec2 S(i * 2 + p.x, p.y + 40);
					ImGui::GetWindowDrawList()->AddCircleFilled(S, 8, beatcolor,8);
					ImGui::GetWindowDrawList()->AddLine(S, ImVec2(S.x, p.y + 100), beatlinecolor,3);

				}

				if (tick)
				{
					ImVec2 S(i * 2 + p.x, p.y + 60);
					ImGui::GetWindowDrawList()->AddCircleFilled(S, 4, tickcolor, 8);
					ImGui::GetWindowDrawList()->AddLine(S, ImVec2(S.x, p.y + 100), ticklinecolor);

				}

			}
			ImGui::GetWindowDrawList()->AddPolyline(Points, 500, IM_COL32(255, 255, 0, 255), false, 2.0f);
			
			ImGui::EndChild();
			ImGui::PushStyleColor(ImGuiCol_Text, loopcolor);
			ImGui::Text("LOOP"); ImGui::SameLine();
			ImGui::PopStyleColor();	
			ImGui::PushStyleColor(ImGuiCol_Text, beatcolor);
			ImGui::Text("BEAT"); ImGui::SameLine();
			ImGui::PopStyleColor();
			ImGui::PushStyleColor(ImGuiCol_Text, tickcolor);
			ImGui::Text("TICK"); ImGui::SameLine();
			ImGui::PopStyleColor();
			ImGui::PushStyleColor(ImGuiCol_Text, clkcolor);
			ImGui::Text("CLOCK"); 
			ImGui::PopStyleColor();
			int32_t textcolo = ImColor::HSV(0, 0, 0.9);
			ImGui::PushStyleColor(ImGuiCol_Text, textcolo);
			char str[4][10] = { "1:1","1:2", "16ppqn", "24ppqn" };
			ImGui::Text("master clock mode: \"%s\"", str[TuesdaySettings.ClockSubDivMode]);
			ImGui::Text("%d ticks-per-beat", (int)TuesdaySettings.tpboptions[TuesdayParams.tpbopt]);
			ImGui::Text("%d beats-per-loop", (int)TuesdaySettings.beatoptions[TuesdayParams.beatopt]);
			ImGui::PopStyleColor();

			


			ImGui::PopStyleColor();
			ImGui::End();
			ImGui::PopStyleColor();
			ImGui::PopFont();
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
