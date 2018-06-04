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

EdgeCutter2_Envelope EnvelopeStatic;

static int outleds[20] = { 0 };
static int targetleds[20] = { 0 };

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
ImVec2 Points4[1000];
ImVec2 Points2[1000];
ImVec2 Points3[1000];
ImVec2 Points3l[1000];
ImVec2 PointsEnvRange[1000];
ImVec2 PointsEnvRange2[1000];
ImVec2 PointsA[1000];
ImVec2 PointsAc[1000];
ImVec2 PointsV[1000];
ImVec2 PointsVSnH[1000];
ImVec2 PointsVCur[1000];

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



extern "C" {
	extern int lowpassenabled;
}

int32_t exptabshort[11] = {1, 11, 32, 69, 138, 266, 502, 938, 1744, 3234, 5986};
int32_t exptablong[11] = {1, 55, 157, 344, 689, 1328, 2509, 4691, 8722, 16170, 29934};

int32_t EdgeCutter2_EnvelopeLength2(int inp, int speed)
{
	if (speed)
	{

		return LERP16(exptabshort, 10, inp);
	}
	else
	{
		return LERP16(exptablong, 10, inp);
	}
}




res_Struct res[10000];
EdgeCutter2_Calibration Calib = { 0,0 };
int main(int, char**)
{
	for (int i = 0; i < 11; i++)
	{
		//0.1 + 0.012276 e ^ (6.14 x)
		float E =  (0.012276  * exp(6.14f * i*0.1)- 0.012276)*(2999/ 5.69672163587);
		exptabshort[i] =(int)(1+ E * 0.001 * 1500.0f);
		float E2 = (0.012276  * exp(6.14f * i*0.1) - 0.012276)*(14999 / 5.69672163587);
		exptablong[i] = (int)(1 + E2 * 0.001 * 1500.0f);
		printf("%f\n", E);
	};

	int i = EdgeCutter2_EnvelopeLength(0, 0);
	EuroRack_InitCalibration();
	EdgeCutter2_Init(&Envelope);
	EdgeCutter2_Init(&EnvelopeStatic);
	

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
	SDL_Window *window = SDL_CreateWindow("TiNRS Edgecutter Inspection GUI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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
	static int updaterate = 4;
	static int T = 0;
	static bool externalclock = true;
	static int clocktiming = 10;
	float fA=0.2, fD=0.3, fS=0.6, fR=0.6, fCurve =0.4, fVelocity = 1.0f;
	
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
	static bool waveoutputs = false;
	static bool staticenv = false;
	static bool docurvetest = false;
	static bool velocityrepeater = false;
	static bool stepped = false;
	static bool rangedisplay = true;

	while (!done)
	{
		SDL_Event event;
		int eventshandled = 0;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSdlGL3_ProcessEvent(&event);
			eventshandled++;
			
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
				ImGui::MenuItem("Static Envelope", NULL, &staticenv);
				ImGui::MenuItem("Docurve Test", NULL, &docurvetest);
				ImGui::MenuItem("Envelope Range display", NULL, &rangedisplay);
				ImGui::MenuItem("Velocity & Repeat Attack Test", NULL, &velocityrepeater);
				ImGui::MenuItem("SteppedResult", NULL, &stepped);

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
					ImGui::SliderInt("lowpass enabled", &lowpassenabled, 0, 1);
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
	
		
//		adcchannels[ADC_A] = 0xade0;
//		adcchannels[ADC_D] = 0xd020;
//		adcchannels[ADC_S] = 0xf1d0;
//		adcchannels[ADC_R] = 0xf0e0 ;


		adcchannels[ADC_CURVE] = (int)((1 - fCurve) * 0xffff);;
		adcchannels[ADC_VELOCITY] = (int)((1 - fVelocity) * 0xffff);;


		EnvelopeStatic.A = Envelope.A = ~(adcchannels[ADC_A]);
		EnvelopeStatic.D = Envelope.D = ~(adcchannels[ADC_D]);
		EnvelopeStatic.S = Envelope.S = ~(adcchannels[ADC_S]);
		EnvelopeStatic.R = Envelope.R = ~(adcchannels[ADC_R]);
		EnvelopeStatic.Curvature = Envelope.Curvature = ~(adcchannels[ADC_CURVE] );
		EnvelopeStatic.Velocity = Envelope.Velocity = ~(adcchannels[ADC_VELOCITY] );

		
	
		
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
			EdgeCutter2_GetEnv(&Envelope, &Params, &Calib);

			for (int i = 0; i<13; i++)
			{
				targetleds[i] = Envelope.StateLeds[i];
				if (outleds[i] < targetleds[i]) outleds[i] = Envelope.StateLeds[i];;
			}




			res[cursor].linout = Envelope.LinearOutput;
			res[cursor].curveout = Envelope.CurvedOutput;
			

			cursor = (cursor + 1) % 10000;
		}

		for (int i = 0; i < 20; i++)
		{
			if (targetleds[i] > outleds[i])
			{
				outleds[i]++;
			}
			else
			{
				if (targetleds[i] < outleds[i])
				{

					outleds[i] -= __min(outleds[i], 10);
				}
			}
		}
		ImVec2 p;

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
			for (int i = 0; i < 65536; i += 256)
			{
				SteppedResult16_t T;
				GetSteppedResult16(i, stepcount, &T);
				uint32_t y = T.index;
				y *= 0xffff;
				y += T.fractional;
				int yy = y / (stepcount*255);
				int xx = (i * 2)/256;

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


		if (docurvetest)
		{
			ImGui::PushFont(pFontBold);
		//	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 255));
			ImGui::Begin("DoCurve Test", &docurvetest, ImGuiWindowFlags_AlwaysAutoResize);

			static int docurvecurvature= 0;
			static int docurvevalue = 0;

			ImGui::SliderInt("DoCurve Curvature", &docurvecurvature, 0, 0xffff);
			ImGui::SliderInt("DoCurve Value", &docurvevalue, 0, 0xffff);
			
			SteppedResult16_t curve;
			
			GetSteppedResult16(docurvecurvature, 4, &curve);

			ImGui::LabelText("labls","%d", DoCurve(0, 65536, docurvevalue, docurvecurvature, &curve, 0x8000));
			static bool showgraph = true;
			ImGui::Checkbox("showgraph", &showgraph);
			if (showgraph)
			{


				ImGui::BeginChild("EnvelopeFrame", ImVec2(540, 257), true);
				p = ImGui::GetCursorScreenPos();
				int Ps[500];
				for (int i = 0; i < 500; i++)
				{
					int32_t P = (DoCurve(0, 32768, (i * 0x8000) / 500, docurvecurvature, &curve, 0x8000));
					Ps[i] = 200 - (P / 327);
					if (P > maxP) maxP = P;
					if (P < minP) minP = P;
					Points4[i].x = i + p.x;
					Points4[i].y = p.y + Ps[i];
				}


				ImGui::GetWindowDrawList()->AddPolyline(Points4, 500, IM_COL32(0, 0, 0, 255), false, 2.0f);
				ImGui::EndChild();
			}


			ImGui::End();
		//	ImGui::PopStyleColor();
			ImGui::PopFont();

		}

		if (rangedisplay)
		{
			ImGui::PushFont(pFontBold);
			//	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 255));
			ImGui::Begin("Envelope Range Display", &rangedisplay, ImGuiWindowFlags_AlwaysAutoResize);

			static bool EnvelopeRange = false;
			ImGui::Checkbox("Long", &EnvelopeRange);

			
				ImGui::BeginChild("EnvelopeRangeFrame", ImVec2(540, 287), true);
				p = ImGui::GetCursorScreenPos();
				int mmaxP = -10000;
				int mminP = 100000;
				int Ps[500];
				int Ps2[500];
				for (int i = 0; i < 500; i++)
				{
					int32_t P = EdgeCutter2_EnvelopeLength(i*0xffff/500,EnvelopeRange?1:0);
					int32_t P2 = EdgeCutter2_EnvelopeLength2(i * 0xffff / 500, EnvelopeRange ? 1 : 0);
					Ps[i] = P;
					Ps2[i] = P2;
					if (P > mmaxP) mmaxP = P;
					if (P < mminP) mminP = P;
				}
				for (int i = 0; i < 500; i++)
				{
					PointsEnvRange[i].x = i + p.x;
					PointsEnvRange[i].y = p.y + 250-((Ps[i] - mminP)*230)/(mmaxP-mminP);
					PointsEnvRange2[i].x = i + p.x;
					PointsEnvRange2[i].y = p.y + 250-((Ps2[i] - mminP) * 230) / (mmaxP - mminP);

				}

				ImGui::GetWindowDrawList()->AddPolyline(PointsEnvRange, 500, IM_COL32(0, 0, 0, 255), false, 2.0f);
				ImGui::GetWindowDrawList()->AddPolyline(PointsEnvRange2, 500, IM_COL32(255, 0, 0, 255), false, 2.0f);
				ImGui::EndChild();
			


			ImGui::End();
			//	ImGui::PopStyleColor();
			ImGui::PopFont();

		}

		if (velocityrepeater)
		{
			ImGui::PushFont(pFontBold);
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 255));
			ImGui::Begin("Velocity Repeater", &velocityrepeater, ImGuiWindowFlags_AlwaysAutoResize);
			static int velocitycurvemode = 1;
			ImGui::BeginChild("RepeaterFrame", ImVec2(540 * 2, 410), true);
			int VelLevel = 0;
			int SampledVel = 0;
			p = ImGui::GetCursorScreenPos();
			ImGui::SliderInt("velocity curve", &velocitycurvemode, 0, 2);
			static bool resetbetweenframes = true;
			if (resetbetweenframes)
			{
				EdgeCutter2_Init(&EnvelopeStatic);
			}
			for (int i = 0; i < 1000; i++)
			{
				switch (velocitycurvemode)
				{
				case 0:VelLevel = (i * 65536) / 1000; break;
				case 1:VelLevel = 65535-( (i * 65536) / 1000); break;
				case 2:VelLevel = floor(sin(i/40.0f )*20767 + 32767); break;

				}
				EnvelopeStatic.Velocity = VelLevel;
				if (i%100 ==0)
				{
					SampledVel = VelLevel;
					EdgeCutter2_Trigger(&EnvelopeStatic, 1, &Params);
				}
				if (i%100 == 50)
				{
					EdgeCutter2_Trigger(&EnvelopeStatic, 0, &Params);
				}
				PointsVSnH[i / 2].x = p.x + i * 2;
				PointsA[i / 2].x = p.x + i * 2;
				PointsAc[i / 2].x = p.x + i * 2;
				PointsV[i / 2].x = p.x + i * 2;
				PointsVCur[i / 2].x = p.x + i * 2;;
				for (int j = 0; j< updaterate + 1; j++) EdgeCutter2_GetEnv(&EnvelopeStatic, &Params, &Calib);
				int P2 = EnvelopeStatic.CurvedOutput;
				int P = EnvelopeStatic.LinearOutput;
				PointsA[i / 2].y = p.y + 400 - ((P ) / (4096/370));
				PointsAc[i / 2].y = p.y + 400 - ((P2) / (4096 / 370));
				PointsV[i / 2].y = p.y + 400 - (VelLevel / (65535/390));
				PointsVSnH[i / 2].y = p.y + 400 - (SampledVel / (65535 / 390));
				PointsVCur[i / 2].y = p.y + 400 - (EnvelopeStatic.VelocityCurrent / (65535 / 390));
			}

			ImGui::GetWindowDrawList()->AddPolyline(PointsVSnH, 500, IM_COL32(100, 255, 20, 255), false, 2.0f);
			ImGui::GetWindowDrawList()->AddPolyline(PointsVCur, 500, IM_COL32(0, 100, 100, 255), false, 3.0f);
			ImGui::GetWindowDrawList()->AddPolyline(PointsV, 500, IM_COL32(20, 100, 255, 255), false, 2.0f);
			ImGui::GetWindowDrawList()->AddPolyline(PointsA, 500, IM_COL32(255, 100, 20, 255), false, 2.0f);
			ImGui::GetWindowDrawList()->AddPolyline(PointsAc, 500, IM_COL32(255, 255, 20, 255), false, 2.0f);
			ImGui::EndChild();


			ImGui::End();
			ImGui::PopStyleColor();
			ImGui::PopFont();
		}

		EnvelopeStatic.Velocity = Envelope.Velocity = ~(adcchannels[ADC_VELOCITY]);

		if (staticenv)
		{
			ImGui::PushFont(pFontBold);
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 255));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255, 255, 100, 255));
			ImGui::Begin("Static Envelope", &staticenv, ImGuiWindowFlags_AlwaysAutoResize);


			SteppedResult16_t curve;

			GetSteppedResult16(EnvelopeStatic.Curvature, 4, &curve);


			ImGui::BeginChild("EnvelopeFrame", ImVec2(540*2, 257*2), true);
			ImGui::Text("%x %x", curve.index, curve.fractional);
			p = ImGui::GetCursorScreenPos();
			static bool resetbetweenframes = true;
			if (resetbetweenframes) EdgeCutter2_Init(&EnvelopeStatic);
			for (int i = 0; i < 1000; i++)
			{
				if (i == 10)
				{
					EdgeCutter2_Trigger(&EnvelopeStatic, 1, &Params);
				}
				if (i == 300)
				{
					EdgeCutter2_Trigger(&EnvelopeStatic, 0, &Params);
				}
				if (i == 400)
				{
					EdgeCutter2_Trigger(&EnvelopeStatic, 1, &Params);
				}
				Points3[i/2].x = p.x+ i*2;
				Points3l[i / 2].x = p.x + i * 2;
				for (int j =0 ;j< updaterate +1;j++) EdgeCutter2_GetEnv(&EnvelopeStatic, &Params, &Calib);
				int P = EnvelopeStatic.CurvedOutput;
				int Pl = EnvelopeStatic.LinearOutput;
				Points3[i / 2].y = p.y + 200 - ((P - 2048) / 10);
				Points3l[i / 2].y = p.y + 200 - ((Pl - 2048) / 10);
			}

			ImGui::GetWindowDrawList()->AddPolyline(Points3, 500, IM_COL32(100, 255, 20, 255), false, 2.0f);
			ImGui::GetWindowDrawList()->AddPolyline(Points3l, 500, IM_COL32(100, 255, 255, 255), false, 2.0f);
			ImGui::EndChild();


			ImGui::End();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopFont();
		}

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
			
			for (int i = 0; i < 13; i++)
			{
				ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(p.x + 8 + (16 * i), p.y +8), 6, IM_COL32(outleds[i], outleds[i], 50, 255));
			}

			ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + 8 + 16 * 0, p.y + 8+32), ImVec2(p.x + 8 + 16 * 3, p.y + 8), IM_COL32(255,255,0,255),2);
			ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + 8 + 16 * 3, p.y + 8 ), ImVec2(p.x + 8 + 16 * 6, p.y + 8+16), IM_COL32(255, 255, 0, 255), 2);
			ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + 8 + 16 * 6, p.y + 8 + 16), ImVec2(p.x + 8 + 16 * 9, p.y + 8+16), IM_COL32(255, 255, 0, 255), 2);
			ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x + 8 + 16 * 9, p.y + 8 + 16), ImVec2(p.x + 8 + 16 * 12, p.y + 8+32), IM_COL32(255, 255, 0, 255), 2);

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
		if (eventshandled == 0) SDL_Delay(4);;
	}

	ImGui_ImplSdlGL3_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
