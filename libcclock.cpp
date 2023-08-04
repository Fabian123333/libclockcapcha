#include <cstdio>
#include <stdlib.h> 
#include <cstring>
#include <cmath>
#include <iostream>

class LibCClock{
	typedef struct RelativePositionStruct{
		float x;
		float y;
	} RelativePosition;
	
	typedef struct ClockPointerStruct{
		RelativePositionStruct pos1;
		RelativePositionStruct pos2;
		RelativePositionStruct pos3;
		RelativePositionStruct pos4;
	} PointerStruct;
	
	public:
		typedef struct CapchaFormat{
			char* content;
			char* solution;
			const char* inputType = "time";
		} Format;
	
		char* key;
		int width = 600;
		int height = 600;

		CapchaFormat Generate(){

			srand(mix(clock(), time(NULL), getpid()));
			
			Format capcha;
			capcha.solution = (char*)malloc(6);
			
			int hour = rand() % 12;
			int minute = rand() % 12 * 5;
			
			sprintf(capcha.solution, "%i:%i", hour, minute);
			
			char* output = (char*)malloc(4096);
			strcpy(output, GenerateSVGHeader());
			strcat(output, GenerateSVGTitle());
			strcat(output, GenerateSVGBackground());
			strcat(output, GenerateSVGClock());
			strcat(output, GenerateSVGPointers());
			strcat(output, GenerateSVGHourPointer(hour + minute / 60));
			strcat(output, GenerateSVGMinutePointer(minute));
			strcat(output, GenerateSVGFooter());
			
			capcha.content = output;
			
			return capcha;
		}
	
	private:
		unsigned long mix(unsigned long a, unsigned long b, unsigned long c)
		{
		    a=a-b;  a=a-c;  a=a^(c >> 13);
		    b=b-c;  b=b-a;  b=b^(a << 8);
		    c=c-a;  c=c-b;  c=c^(b >> 13);
		    a=a-b;  a=a-c;  a=a^(c >> 12);
		    b=b-c;  b=b-a;  b=b^(a << 16);
		    c=c-a;  c=c-b;  c=c^(b >> 5);
		    a=a-b;  a=a-c;  a=a^(c >> 3);
		    b=b-c;  b=b-a;  b=b^(a << 10);
		    c=c-a;  c=c-b;  c=c^(b >> 15);
		    return c;
		}
			
				  
		RelativePositionStruct RotatePointByAngle(RelativePositionStruct point, float angle){
			float s = sin(angle);
			float c = cos(angle);
			
			RelativePositionStruct pointNew;
			
			point.x -= 0.5;
			point.y -= 0.5;
			
			pointNew.x = point.x * c - point.y * s;
			pointNew.y = point.x * s + point.y * c;
			
			pointNew.x += 0.5;
			pointNew.y += 0.5;
			
			return pointNew;
		}
	
		char* GenerateSVGHeader(){
			char* output =(char*)malloc(192);
			sprintf(output, "<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"%i\" height=\"%i\" viewBox=\"0 0 %i %i\">", width, height, width, height);	
			return output;
		}
	
		char* GenerateSVGBackground(){
			char* output =(char*)malloc(64);
			sprintf(output, "<rect x=\"0\" y=\"0\" width=\"%i\" height=\"%i\" fill=\"grey\"/>", width, height);
			return output;
		}
	
		char* GenerateSVGHourPointer(float hour){
			char* output =(char*)malloc(64);
			
			PointerStruct pointer = {{0.5, 0.45}, {0.55, 0.35}, {0.5, 0.2}, {0.45, 0.35}};
			float angle = hour * 30 * 3.14159 / 180;
			
			PointerStruct pointer_new = {RotatePointByAngle(pointer.pos1, angle), RotatePointByAngle(pointer.pos2, angle), RotatePointByAngle(pointer.pos3, angle), RotatePointByAngle(pointer.pos4, angle)};
			
			return DrawPointer(pointer_new);
		}
	
		char* GenerateSVGMinutePointer(float minute){
			char* output =(char*)malloc(64);
			
			PointerStruct pointer = {{0.5, 0.45}, {0.53, 0.30}, {0.5, 0.1}, {0.47, 0.30}};
			float angle = minute * 6 * 3.14159 / 180;
			
			PointerStruct pointer_new = {RotatePointByAngle(pointer.pos1, angle), RotatePointByAngle(pointer.pos2, angle), RotatePointByAngle(pointer.pos3, angle), RotatePointByAngle(pointer.pos4, angle)};
			
			return DrawPointer(pointer_new);
		}
	
		char* DrawPointer(PointerStruct pointer){
			char* color = (char*)malloc(10);
			char* output = (char*)malloc(129);
			int x_offset = 0;
			int y_offset = 0;
			
			char* startMarker = (char*)malloc(32);
			char* l1 = (char*)malloc(64);
			char* l2 = (char*)malloc(64);
			char* l3 = (char*)malloc(64);
			
			switch(rand() % 4){
				case 0:
					strcpy(color, "black");
					break;
				case 1:
					strcpy(color, "white");
					break;
				case 2:
					strcpy(color, "blue");
					break;
				case 3:
					strcpy(color, "brown");
					break;
			}
			
			int startMarker_x = std::floor(pointer.pos1.x * width) + x_offset;
			int startMarker_y = std::floor(pointer.pos1.y * height) + y_offset;
			
			//printf("startMarker_x %i startMarker_y %i pointer.pos1.x %f width %i\n", startMarker_x, startMarker_y, pointer.pos1.x, width);
			
			sprintf(startMarker, "m%i,%i", startMarker_x, startMarker_y);
			
			int l1_x = std::floor(width * (pointer.pos2.x - pointer.pos1.x)) + x_offset;
			int l1_y = std::floor(height * (pointer.pos2.y - pointer.pos1.y)) + y_offset;
			
			char* l1_s = (char*)malloc(3);
			strcpy(l1_s, l1_y >= 0?",":"");
			
			sprintf(l1, "l%i%s%i", l1_x, l1_s, l1_y);
			
			int l2_x = std::floor(width * (pointer.pos3.x - pointer.pos2.x)) + x_offset;
			int l2_y = std::floor(height * (pointer.pos3.y - pointer.pos2.y)) + y_offset;
			
			char* l2_s = (char*)malloc(3);
			strcpy(l2_s, l2_y >= 0?",":"");
			
			sprintf(l2, "l%i%s%i", l2_x, l2_s, l2_y);
			
			int l3_x = std::floor(width * (pointer.pos4.x - pointer.pos3.x)) + x_offset;
			int l3_y = std::floor(height * (pointer.pos4.y - pointer.pos3.y)) + y_offset;
			
			char* l3_s = (char*)malloc(3);
			strcpy(l3_s, l3_y >= 0?",":"");
			
			sprintf(l3, "l%i%s%i", l3_x, l3_s, l3_y);
			
			sprintf(output, "<path id=\"pointer\" d=\"%s %s %s %s\" fill=\"%s\"/>", startMarker, l1, l2, l3, color);
			
			free(l1);
			free(l2);
			free(l3);
			free(startMarker);
			free(color);
			free(l1_s);
			free(l2_s);
			free(l3_s);
			
			return output;
		}
	
		char* GenerateSVGPointers(){
			char* output = (char*)malloc(2048);
			strcpy(output, "");
			
			PointerStruct pointers[12] = {
				//12
				{{0.48, 0.30}, {0.52, 0.30}, {0.52, 0.10}, {0.48, 0.10}},
				{{0.582679491924311,0.316794919243112}, {0.617320508075689,0.336794919243112}, {0.717320508075689,0.163589838486225}, {0.682679491924311,0.143589838486225}},
				{{0.663205080756888,0.382679491924311}, {0.683205080756888,0.417320508075689}, {0.856410161513775,0.317320508075689}, {0.836410161513775,0.282679491924311}},
				//3
				{{0.7, 0.48}, {0.7, 0.52}, {0.9, 0.52}, {0.9, 0.48}},
				{{0.683205080756888,0.582679491924311}, {0.663205080756888,0.617320508075689}, {0.836410161513775,0.717320508075689}, {0.856410161513775,0.682679491924311}},
				{{0.617320508075689,0.663205080756888}, {0.582679491924311,0.683205080756888}, {0.682679491924311,0.856410161513775}, {0.717320508075689,0.836410161513775}},
				//6
				{{0.48, 0.70}, {0.52, 0.70}, {0.52, 0.90}, {0.48, 0.90}},
			
				{{0.266, 0.83}, {0.3, 0.85}, {0.4, 0.677}, {0.366, 0.66}},
				{{0.336794919243112,0.617320508075689}, {0.316794919243112,0.582679491924311}, {0.143589838486225,0.682679491924311}, {0.163589838486225,0.717320508075689}},
				
				//9
				{{0.3, 0.48}, {0.3, 0.52}, {0.1, 0.52}, {0.1, 0.48}},
				{{0.316794919243112,0.417320508075689}, {0.336794919243112,0.382679491924311}, {0.163589838486225,0.282679491924311}, {0.143589838486225,0.317320508075689}},
				{{0.3826, 0.3368}, {0.41732, 0.3168}, {0.3173, 0.144}, {0.28268, 0.1636}}
			};
			
			for(int i = 0; i < sizeof(pointers)/sizeof(pointers[0]); i++){
				char* pointerString = DrawPointer(pointers[i]);
				
				strcat(output, pointerString);
				free(pointerString);
			}
			
			return output;
		}
	
		char* GenerateSVGClock(){
			char* output =(char*)malloc(256);
			
			int cx_offset = std::floor((float) rand()/RAND_MAX * (width * 0.1) - (0.05 * width));
			int cx = std::floor(width / 2) + cx_offset;
			
			int cy_offset = std::floor((float) rand()/RAND_MAX * (height * 0.1) - (0.05 * height));
			int cy = std::floor(height / 2) + cy_offset;
			
			int rx_offset = std::floor((float) rand()/RAND_MAX * (width * 0.05));
			int ry_offset = std::floor((float) rand()/RAND_MAX * (height * 0.05));
			
			int rx = ((std::floor(width / 2) - rx_offset));
			int ry = ((std::floor(height / 2) - ry_offset));
			
			int rx_inner_offset = std::floor(rx_offset + width * 0.02 - (float) rand()/RAND_MAX * width * 0.01);
			int ry_inner_offset = std::floor(ry_offset + height * 0.02 - (float) rand()/RAND_MAX * height * 0.01);
			
			sprintf(output, "<ellipse id=\"r1\" cx=\"%i\" cy=\"%i\" rx=\"%i\" ry=\"%i\" fill=\"#231709\"/><ellipse id=\"r2\" cx=\"%i\" cy=\"%i\" rx=\"%i\" ry=\"%i\" fill=\"grey\"/>", cx, cy, rx, ry, cx, cy, rx - rx_inner_offset, ry - ry_inner_offset);
			return output;
		}
	
		char* GenerateSVGTitle(){
			char* output =(char*)malloc(64);
			sprintf(output, "<title>Capcha-ID %s</title>", key);	
			return output;
		}
	
	
		char* GenerateSVGFooter(){
			char* output = (char*)malloc(7);
			sprintf(output, "</svg>");
			return output;	
		}
};
/*
int main(){
	LibCClock capcha = LibCClock();
	
	printf(capcha.Generate().content);
}*/
