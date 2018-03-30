// ImGui - standalone example application for SDL2 + OpenGL
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)
// (GL3W is a helper library to access OpenGL functions since there is no standard header to access modern OpenGL functions easily. Alternatives are GLEW, Glad, etc.)

#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"
#include <stdio.h>
#include <GL/gl3w.h>    // This example is using gl3w to access OpenGL functions (because it is small). You may use glew/glad/glLoadGen/etc. whatever already works for you.
#include <SDL.h>
#include <vector>

#include "../../WobblerV2/Sources/Wobbler2.h"

Wobbler2_LFO_t LFO2Static;
Wobbler2_Settings LFO2Settings;
Wobbler2_Params LFO2Params;
enum
{
	ADC_MODULATION,
	ADC_PHASING,
	ADC_SHAPE,
	ADC_SPEED,
	ADC_AMTNORMAL,
	ADC_AMTPHASED,
	ADC_Count
};

typedef struct result
{
	int32_t normal;
	int32_t phased;
	int32_t envelope1;
	int32_t envelope2;
	int32_t sync;
}result;

result res[10000];
int cursor = 0;
ImVec2 Points[1000];
ImVec2 EnvPoints[1000];
ImVec2 Points2[1000];

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

int main(int, char**)
{
	Wobbler2_Init(&LFO2Static);
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
	SDL_Window *window = SDL_CreateWindow("TiNRS Wobbler Inspection GUI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	
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
	//ImGui::StyleColorsClassic();

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'misc/fonts/README.txt' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	//bool show_demo_window = true;
	//bool show_another_window = false;
	ImVec4 clear_color = ImVec4(1.0f / 255.0f, 58.0f / 255.0f, 66.0f / 255.0f, 1.00f);

	// racing turqoise: 1,58,66
	// haringmuts geel: 255, 234,0 

#define RACINGGREEN IM_COL32(1, 58, 66, 255)

	// Main loop
	bool done = false;

	static float fPhase = 1.0f;
	static float fMod = 0.1f;
	static float fShape = 0.3f;
	static float fSpeed = 0.6f;
	static float f5 = 0.0f;
	static float f6 = 0.0f;
	static int counter = 0;
	static int updaterate = 1;
	static bool simsink = true;
	static int SyncInterval = 40;;

	int SyncT = 0;

	ImFont* pFont = io.Fonts->AddFontFromFileTTF("Fontfabric - Panton.otf", 14.0f);
	ImFont* pFontBold = io.Fonts->AddFontFromFileTTF("Fontfabric - Panton ExtraBold.otf", 18.0f);

	unsigned char * pixels;
	int width, height, bytes_per_pixels;
	//ImGui::GetStyle() = 0.3;
	ImGui::GetStyle().ItemInnerSpacing = ImVec2(5, 5);;
	ImGui::GetStyle().ItemSpacing = ImVec2(5, 5);
	ImGui::GetStyle().FramePadding = ImVec2(5, 5);
	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 1.0f, 1.0f, .800f);
	
	while (!done)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSdlGL3_ProcessEvent(&event);
			if (event.type == SDL_QUIT) done = true;
		}
		ImGui_ImplSdlGL3_NewFrame(window);
		static bool waveforms = true;
		static bool parameters = true;
		static bool envelopedisplay = true;
		static bool stepped = false;
		if (ImGui::BeginMainMenuBar())
		{

			if (ImGui::BeginMenu("Wobbler Windows"))
			{
				ImGui::MenuItem("Output Waveforms", NULL, &waveforms);
				ImGui::MenuItem("Wobbler Parameters", NULL, &parameters);
				ImGui::MenuItem("FancyEnvelope", NULL, &envelopedisplay);
				ImGui::MenuItem("SteppedResult", NULL, &stepped);

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if (parameters)
		{ 
			ImGui::PushFont(pFontBold);
		
			ImGui::Begin("Wobbler Parameters",&parameters, ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::PushFont(pFont);



		

			ImGui::SliderFloat("Phase", &fPhase, 0.0f, 1.0f);
			ImGui::SliderFloat("Mod", &fMod, 0.0f, 1.0f);
			ImGui::SliderFloat("Shape", &fShape, 0.0f, 1.0f);
			ImGui::SliderFloat("Speed", &fSpeed, 0.0f, 1.0f);

			ImGui::SliderFloat("Amount 1", &f5, 0.0f, 1.0f);

			ImGui::SliderFloat("Amount 2", &f6, 0.0f, 1.0f);





			if (ImGui::Button("Trigger"))
			{
				Wobbler2_Trigger(&LFO2Static, 0, &LFO2Params);
				Wobbler2_Trigger(&LFO2Static, 1, &LFO2Params);
			}
			ImGui::SameLine();
			if (ImGui::Button("Sync"))
			{
				Wobbler2_SyncPulse(&LFO2Static);
			}
			ImGui::Checkbox("Simulate Syncpulse", &simsink);
			if (simsink)
			{
				ImGui::SameLine();
				ImGui::SliderInt("Sync interval", &SyncInterval, 1, 300);
			}

			if (ImGui::Button("Basic 1"))                      fShape = (0.0f / 6.0f);
			ImGui::SameLine();
			if (ImGui::Button("Basic 2"))                      fShape = (1.0f / 6.0f);
			ImGui::SameLine();
			if (ImGui::Button("Phased"))                      fShape = (2.0f / 6.0f);
			ImGui::SameLine();
			if (ImGui::Button("Twang"))                      fShape = (3.0f / 6.0f);
			ImGui::SameLine();
			if (ImGui::Button("Pendulum"))                      fShape = (4.0f / 6.0f);
			ImGui::SameLine();
			if (ImGui::Button("SnH 1"))                      fShape = (5.0f / 6.0f);
			ImGui::SameLine();
			if (ImGui::Button("SnH 2"))                      fShape = (6.0f / 6.0f);

			ImGui::SliderInt("Update Speed", &updaterate, 0, 10);




			ImVec2 p = ImGui::GetCursorScreenPos();


			float R = 5;
			for (int i = 0; i < 9; i++)
			{
				int lA = LFO2Static.Led[0][i];
				int lB = LFO2Static.Led[1][i];
				ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(10 + p.x + i * 15, p.y + 10), R, IM_COL32(lA, lA, 60, 255));
				ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(10 + p.x + i * 15, p.y + 30), R, IM_COL32(lB, lB, 60, 255));
			}

			for (int i = 0; i < 5; i++)
			{
				int lA = LFO2Static.ModeLed[i];
				ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(10 + p.x + i * 15, p.y + 50), R, IM_COL32(lA, lA, 60, 255));
			}
				
			
			ImGui::PopFont();
			ImGui::End();
			ImGui::PopFont();

		}
		int minN = 100000;
		int maxN = -100000;
		int minP = 100000;
		int maxP = -100000;

		uint16_t adcchannels[ADC_Count];
		adcchannels[ADC_PHASING] = (int)((1 - fPhase) * 0xffff);
		adcchannels[ADC_MODULATION] = (int)((1 - fMod) * 0xffff);
		adcchannels[ADC_SHAPE] = (int)((1 - fShape) * 0xffff);
		adcchannels[ADC_SPEED] = (int)((1 - fSpeed) * 0xffff);;
		adcchannels[ADC_AMTNORMAL] = (int)((1 - f5) * 0xffff);;
		adcchannels[ADC_AMTPHASED] = (int)((1 - f6) * 0xffff);;

		LFO2Static.Mod = Wobbler2_SkipThe1Percent(~adcchannels[ADC_MODULATION]);
		LFO2Static.Shape = ~adcchannels[ADC_SHAPE];
		LFO2Static.Phasing = (Wobbler2_SkipThe1Percent(adcchannels[ADC_PHASING])) >> 4;
		LFO2Static.Speed = ((0xffff - adcchannels[ADC_SPEED]) >> 7);
		LFO2Static.SpeedOrig = ((0xffff - adcchannels[ADC_SPEED]));
		LFO2Static.Amount1 = ((adcchannels[ADC_AMTNORMAL]) >> 1) - (1 << 14);
		LFO2Static.Amount2 = ((adcchannels[ADC_AMTPHASED]) >> 1) - (1 << 14);
		
		static int synccountdown = 0;
		for (int i = 0; i < updaterate*10; i++)
		{
			res[cursor].sync = synccountdown>0?1:0;
			SyncT++;

			if (simsink)
			{
				if (SyncT >= (SyncInterval*5))
				{
					Wobbler2_SyncPulse(&LFO2Static);;
					SyncT = 0;
					synccountdown = 15;
				}

				
			}
			if (synccountdown > 0) synccountdown--;
			res[cursor].normal = Wobbler2_Get(&LFO2Static, &LFO2Params);
			res[cursor].phased = LFO2Static.OutputPhased;
			res[cursor].envelope1 = LFO2Static.FancyEnv.currentcurved;
			cursor = (cursor + 1) % 10000;
		}
		ImVec2 p;
		if (waveforms)
		{
			ImGui::PushFont(pFontBold);
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 255));
			ImGui::Begin("Wave outputs", &waveforms, ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255, 255, 200, 255));
			ImGui::BeginChild("Normal", ImVec2(500, 320), true);
			 p = ImGui::GetCursorScreenPos();

			for (int i = 0; i < 500; i++)
			{
				if (res[(cursor - 1 - i * 10 + 10000) % 10000].sync)
				{
					ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(p.x + i, p.y + 3), 3, IM_COL32(255, 255, 0, 255), 4);
				}
				int N = res[(cursor - 1 - i * 10 + 10000) % 10000].normal;
				if (N > maxN) maxN = N;
				if (N < minN) minN = N;

				Points[i].x = i + p.x;
				Points[i].y = p.y + 100 + ((N - 2048) / 20);
			}
			ImGui::GetWindowDrawList()->AddPolyline(Points, 500, IM_COL32(255, 255, 0, 255), false, 2.0f);
			for (int i = 0; i < 500; i++)
			{
				int P = res[(cursor - 1 - i * 10 + 10000) % 10000].phased;
				if (P > maxP) maxP = P;
				if (P < minP) minP = P;
				Points2[i].x = i + p.x;
				Points2[i].y = p.y + 100 + ((P - 2048) / 20);
			}
			ImGui::GetWindowDrawList()->AddPolyline(Points2, 500, IM_COL32(100, 255, 20, 255), false, 2.0f);
			int maxE = -1000000;
			int minE = 10000000;
			for (int i = 0; i < 500; i++)
			{
				int N = res[(cursor - 1 - i * 10 + 10000) % 10000].envelope1;
				if (N > maxE) maxE = N;;
				if (N < minE) minE = N;

				EnvPoints[i].x = i + p.x;
				EnvPoints[i].y = p.y + 275 - ((N >> 8));
			}
			ImGui::GetWindowDrawList()->AddPolyline(EnvPoints, 500, IM_COL32(255, 255, 0, 255), false, 2.0f);

			ImGui::EndChild();
			ImGui::Text("N: %d->%d, P: %d->%d, E: %d->%d", minN, maxN, minP, maxP, minE, maxE);



			ImGui::PopStyleColor();
			ImGui::End();
			ImGui::PopStyleColor();
			ImGui::PopFont();
		}
		if (stepped)
		{
			ImGui::PushFont(pFontBold);

			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 255));
			ImGui::Begin("SteppedTest", &stepped, ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255, 255, 200, 255));

			static int stepcount = 6;
			ImGui::SliderInt("steps", &stepcount, 1, 20);
			ImGui::BeginChild("StepptedTestFrame", ImVec2(540, 280), true);



			p = ImGui::GetCursorScreenPos();
			int sMin = 100000;
			int sMax = -100000;
			for (int i = 0; i < 256; i += 4)
			{
				SteppedResult_t T;
				Wobbler2_GetSteppedResult(i << 8, stepcount, &T);
				int yy = (T.index * 256 + T.fractional) / stepcount;
				int xx = i * 2;

				if (yy < sMin) sMin = yy;
				if (yy > sMax) sMax = yy;
				ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(xx + p.x, yy + p.y), 1, IM_COL32(255, 255, 60, 255));

			}
			ImGui::EndChild();
			ImGui::Text("Steps %d: %d->%d", stepcount, sMin, sMax);

			ImGui::PopStyleColor();

			ImGui::End();
			ImGui::PopStyleColor();
			ImGui::PopFont();
		}


		if (envelopedisplay){
			ImGui::PushFont(pFontBold);
			static int envparam = 900;
			


			static float EnvResult[256];
			static float CurvResult[256];
			Wobbler2_FancyEnv_t FancyEnv;
			Wobbler2_FancyEnv_Init(&FancyEnv);
			Wobbler2_FancyEnv_Trigger(&FancyEnv);
			int Range = Wobbler2_LFORange3(envparam, WOBBLERSAMPLERATE) * 2;
			for (int q = 0; q < Range; q++)
			{
				int i = (q * 256) / Range;
				EnvResult[i] = Wobbler2_FancyEnv_Update(&FancyEnv, envparam) / (float)(1 << 24);
				CurvResult[i] = FancyEnv.currentcurved / (float)(1 << 16);
		
			}
		
			ImGui::Begin("EnvelopeShape",&envelopedisplay, ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::PushFont(pFont);

			ImGui::BeginChild("EnvelopeFrame", ImVec2(540, 257), true);
			static int oneperctest = 0;
			ImGui::SliderInt("1% test", &oneperctest, 0, 0xffff); ImGui::SameLine(); ImGui::Text("%x (%f) -> %x", oneperctest,(float)(oneperctest*100.0f)/65535.0f,  Wobbler2_SkipThe1Percent(oneperctest));

			p = ImGui::GetCursorScreenPos();
			for (int i = 0; i < 256; i++)
			{
				float xx = i * 2;
				float yy = 256 - EnvResult[i] * 255;
				float yy2 = 256 - CurvResult[i] * 255;
				ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(xx + p.x, yy + p.y), 1, RACINGGREEN);
				ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(xx + p.x, yy2 + p.y), 1, RACINGGREEN);
			}
			ImGui::EndChild();
			ImGui::SliderInt("Parameter", &envparam, 0, 0xffff);
			ImGui::Text("Range: %d (= %f seconds)", Range, Range / (float)WOBBLERSAMPLERATE);
//			ImGui::SliderFloat("attack lowerbound", &attacklowerbound, -0.99999f, 0.99999f);
//			ImGui::SliderFloat("attack upperbound", &attackupperbound, -0.99999f, 0.99999f);
	//		ImGui::SliderFloat("decay lowerbound", &decaylowerbound, -0.99999f, 0.99999f);
		//	ImGui::SliderFloat("decay upperbound", &decayupperbound, -0.99999f, 0.99999f);
			ImGui::PopFont();
			ImGui::End();
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
