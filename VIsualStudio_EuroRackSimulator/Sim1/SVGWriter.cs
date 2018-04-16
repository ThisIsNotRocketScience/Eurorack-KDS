﻿using GlmNet;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace TINRS_ArtWorkGenerator
{
    public class Polygon
    {

        public byte r, g, b;
        public List<vec2> Vertices = new List<vec2>();
        public int Type = 0;
        public int depth = 0;
        internal bool divided = false;

    }

    public class SVGWriter
    {
        public static void Write(string filename, int w, int h, List<Polygon> Polygons, double strokewidth, bool Closed, Color BG)
        {
            string bgcolorstring = String.Format("#{0:X2}{1:X2}{2:X2}", BG.R, BG.G, BG.B);


            List<string> OutLines = new List<string>();

            OutLines.Add("<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\" >");
            OutLines.Add(String.Format("<svg version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:space=\"preserve\" width=\"{0}\" height=\"{1}\">", w, h));

            OutLines.Add(String.Format("<rect width=\"100%\" height=\"100%\" fill=\"{0}\" />", bgcolorstring));

                 Dictionary <int, List<string>> groups = new Dictionary<int, List<string>>();
            for (int i = 0; i < 256; i++)
            {
                groups[i] = new List<string>();
            }
            List<string> colors = new List<string>();// { "#606060", "#505050", "#404040", "#303030", "#202020", "#101010", "#080808", "#040404", "#020202", "#010101", "#000000", "#000000", "#000000", "#000000", "#000000", "#000000", "#000000", "#000000", "#000000" };
          
            foreach (var a in Polygons)
            {
                string colorstring = String.Format("#{0:X2}{1:X2}{2:X2}", a.r, a.g, a.b);
                string commands = "";
                commands += "M" + a.Vertices[0].x.ToString().Replace(',', '.') + "," + a.Vertices[0].y.ToString().Replace(',', '.');
                for (int i = 1; i < a.Vertices.Count; i++)
                {
                    commands += "L" + a.Vertices[i].x.ToString().Replace(',', '.') + "," + a.Vertices[i].y.ToString().Replace(',', '.');
                }
                if (Closed)
                {
                    commands += "L" + a.Vertices[0].x.ToString().Replace(',', '.') + "," + a.Vertices[0].y.ToString().Replace(',', '.');
                    commands += "Z";
                }
                string setup = String.Format("<path fill=\"none\" stroke=\"{2}\" stroke-width=\"{0}\" stroke-linecap=\"round\" stroke-linejoin=\"round\" d=\"{1}\"/>", strokewidth, commands, colorstring);

                groups[a.depth].Add(setup);
            }

            foreach (var a in groups)
            {
                var L = a.Value;
                if (L.Count > 0)
                {
                    OutLines.Add("<g>");
                    foreach (var p in L) OutLines.Add(p);
                    OutLines.Add("</g>");
                }
            }
            OutLines.Add("</svg>");
            System.IO.File.WriteAllLines(filename, OutLines);
        }
    }
}
