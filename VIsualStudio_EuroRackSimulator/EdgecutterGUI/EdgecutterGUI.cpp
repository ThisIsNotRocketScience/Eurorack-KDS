#include "../../Tuesday/Sources/Tuesday.h"
#include "imgui.h"
#include "../TestFrameImgUI/imgui_impl_sdl_gl3.h"
#include <stdio.h>
#include <GL/gl3w.h>    
#include <SDL.h>
#include "../../EdgeCutterV2/Sources/EdgeCutter2.h" 
#include "../libs/lodepng-master/lodepng.h"

EdgeCutter2_Envelope Envelope;
EdgeCutter2_Settings Settings;
EdgeCutter2_Params Params;



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
	ADC_A,
	ADC_D,
	ADC_S,
	ADC_R,
	ADC_CURVE,
	ADC_VELOCITY,
	ADC_COUNT
};
int cursor = 0;
ImVec2 Points[1000];
ImVec2 EnvPoints[1000];
ImVec2 Points2[1000];

typedef struct res_Struct
{
	bool aled, dled, sled, rled;
	
	int leds[16];
	int modeled[3];
	int speedled[2];
	int linout;
	int curveout;

	
}
res_Struct;

res_Struct res[10000];

int main(int, char**)
{
	
	EuroRack_InitCalibration();

	

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
	float fA=0.2, fD=0.3, fS=0.6, fR=0.6, fCurve =0, fVelocity = 1.0f;
	
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

			if (ImGui::BeginMenu("Edgecutter Windows"))
			{
				ImGui::MenuItem("Output Waveforms", NULL, &waveoutputs);
				ImGui::MenuItem("Edgecutter Parameters", NULL, &parameters);

				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if (parameters)
		{
			ImGui::PushFont(pFontBold);
			{
				ImGui::Begin("Edgecutter Parameters", &parameters, ImGuiWindowFlags_AlwaysAutoResize);
				ImGui::PushFont(pFont);


				{
					ImGui::SliderFloat("Attack", &fA,0,1);
					ImGui::SliderFloat("Decay", &fD, 0, 1);
					ImGui::SliderFloat("Sustain", &fS, 0, 1);
					ImGui::SliderFloat("Release", &fR, 0, 1);
					ImGui::SliderFloat("Curvature", &fCurve, 0, 1);
					ImGui::SliderFloat("Velocity", &fVelocity, 0, 1);
					static bool trigger;
					if (ImGui::Checkbox("Trigger", &trigger)) 
					{
						EdgeCutter2_Trigger(&Envelope, trigger?1:0, &Params);
					}
					if (ImGui::Button("Retrigger"))
					{

					}


					static int  mode = 1;
					ImGui::Text("mode:"); ImGui::SameLine();
					ImGui::RadioButton("oneshot", &mode, 0); ImGui::SameLine();
					ImGui::RadioButton("normal", &mode, 1); ImGui::SameLine();
					ImGui::RadioButton("loop", &mode, 2); 
				
					
					static int  speed = 0;
					ImGui::Text("speed:"); ImGui::SameLine();
					ImGui::RadioButton("slow", &speed, 0); ImGui::SameLine();
					ImGui::RadioButton("fast", &speed, 1); 
					
					
					Params.mode = mode;
					Params.speed = speed;




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
		adcchannels[ADC_A] = (int)((1 - fA) * 0xffff);
		adcchannels[ADC_D] = (int)((1 - fD) * 0xffff);
		adcchannels[ADC_S] = (int)((1 - fS) * 0xffff);
		adcchannels[ADC_R] = (int)((1 - fR) * 0xffff);;
		adcchannels[ADC_CURVE] = (int)((1 - fCurve) * 0xffff);;
		adcchannels[ADC_VELOCITY] = (int)((1 - fVelocity) * 0xffff);;


		Envelope.A = ~(adcchannels[ADC_A] >> 8);
		Envelope.D = ~(adcchannels[ADC_D] >> 8);
		Envelope.S = ~(adcchannels[ADC_S] >> 8);
		Envelope.R = ~(adcchannels[ADC_R] >> 8);
		Envelope.Curvature = ~(adcchannels[ADC_CURVE] >> 8);
		Envelope.Velocity = ~(adcchannels[ADC_VELOCITY] >> 8);

	
		
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
			;
			EdgeCutter2_GetEnv(&Envelope, &Params);
			res[cursor].linout = Envelope.LinearOutput;
			res[cursor].curveout = Envelope.CurvedOutput;
			

			cursor = (cursor + 1) % 10000;
		}
		ImVec2 p;
		if(waveoutputs){
			ImGui::PushFont(pFontBold);
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 255));
			ImGui::Begin("Wave outputs", &waveoutputs, ImGuiWindowFlags_AlwaysAutoResize);

			ImGui::BeginChild("EnvelopeFrame", ImVec2(540, 257), true);

			p = ImGui::GetCursorScreenPos();
			
			for (int i = 0; i < 500; i++)
			{
				int P = res[(cursor - 1 - i * 10 + 10000) % 10000].linout;
				if (P > maxP) maxP = P;
				if (P < minP) minP = P;
				Points2[i].x = 500 - i + p.x;
				Points2[i].y = p.y + 100 - ((P - 2048) / 20);
			}
			ImGui::GetWindowDrawList()->AddPolyline(Points2, 500, IM_COL32(100, 255, 20, 255), false, 2.0f);

			for (int i = 0; i < 500; i++)
			{
				int P = res[(cursor - 1 - i * 10 + 10000) % 10000].curveout;
				if (P > maxP) maxP = P;
				if (P < minP) minP = P;
				Points[i].x = 500-i + p.x;
				Points[i].y = p.y + 100 - ((P - 2048) / 20);
			}
			ImGui::GetWindowDrawList()->AddPolyline(Points, 500, IM_COL32(40, 255, 220, 255), false, 2.0f);

			for (int i = 0; i < 16; i++)
			{
				ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(p.x + 8 + (16 * i), p.y +8), 6, IM_COL32(Envelope.StateLeds[i], Envelope.StateLeds[i], 50, 255));
			}

			ImGui::EndChild();


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
