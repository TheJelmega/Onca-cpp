using System;
using System.Collections.Generic;
using System.IO;

class UnicodeTableGen
{
	static List<(uint, uint)>               _singleMapping = new();

	public static void Main(string[] args)
	{
		ParseCaseFolding(args[0]);
		WriteUnicodeTables(args[1]);
	}

	// Only handles S and C
	public static void ParseCaseFolding(string filename)
	{
		char[] hex = "0123456789aAbBcCdDeEfF".ToCharArray();

		using (StreamReader reader = new StreamReader(filename))
		{
			while (!reader.EndOfStream)
			{
				string line = reader.ReadLine();
				if (string.IsNullOrWhiteSpace(line) || 
				    line.StartsWith("#") ||
				    line.IndexOf(" F; ", StringComparison.Ordinal) != -1 ||
				    line.IndexOf(" T; ", StringComparison.Ordinal) != -1)
					continue;

				int semi = line.IndexOf(';');


				uint upper = uint.Parse(line.Substring(0, semi), System.Globalization.NumberStyles.HexNumber);
				++semi;

				// Skip kind
				semi = line.IndexOf(';', semi);
				++semi;
				int start = line.IndexOfAny(hex, semi);
				semi = line.IndexOf(';', start);

				uint lower = uint.Parse(line.Substring(start, semi - start), System.Globalization.NumberStyles.HexNumber);

				_singleMapping.Add((lower, upper));
			}
		}
	}

	public static void WriteUnicodeTables(string filename)
	{
		using (StreamWriter writer = new StreamWriter(filename, false))
		{
			writer.WriteLine("#pragma once");
			writer.WriteLine("#include \"core/MinInclude.h\"");
			writer.WriteLine("#include \"StringUtilTypes.h\"");
			writer.WriteLine();
			writer.WriteLine("namespace Core::Unicode");
			writer.WriteLine("{");

			writer.WriteLine($"\tconstexpr usize UnicodeCaseTableSize = {_singleMapping.Count};");

			writer.WriteLine( "\tconstexpr UCodepoint UnicodeCaseLowerTable[UnicodeCaseTableSize] =");
			writer.WriteLine( "\t{");

			for (int i = 0; i < _singleMapping.Count;)
			{
				writer.Write($"\t\t");

				for (int j = 0; j < 8 && i < _singleMapping.Count; ++j, ++i)
					writer.Write($"0x{_singleMapping[i].Item1:X5}, ");

				writer.WriteLine();
			}

			writer.WriteLine( "\t};");

			writer.WriteLine();

			writer.WriteLine("\tconstexpr UCodepoint UnicodeCaseUpperTable[UnicodeCaseTableSize] =");
			writer.WriteLine("\t{");

			for (int i = 0; i < _singleMapping.Count;)
			{
				writer.Write($"\t\t");

				for (int j = 0; j < 8 && i < _singleMapping.Count; ++j, ++i)
					writer.Write($"0x{_singleMapping[i].Item1:X5}, ");

				writer.WriteLine();
			}
			writer.WriteLine("\t};");

			writer.WriteLine("\tconstexpr Utf8Char Utf8CaseLowerTable[UnicodeCaseTableSize] =");
			writer.WriteLine("\t{");

			for (int i = 0; i < _singleMapping.Count;)
			{
				writer.Write($"\t\t");

				for (int j = 0; j < 4 && i < _singleMapping.Count; ++j, ++i)
				{
					var (bytes, size) = UnicodeToUtf8(_singleMapping[i].Item1);
					writer.Write($"{{ {{ 0x{bytes[0]:X2}, 0x{bytes[1]:X2}, 0x{bytes[2]:X2}, 0x{bytes[3]:X2} }}, {size} }}, ");
				}
					

				writer.WriteLine();
			}

			writer.WriteLine("\t};");

			writer.WriteLine("\tconstexpr Utf8Char Utf8CaseUpperTable[UnicodeCaseTableSize] =");
			writer.WriteLine("\t{");

			for (int i = 0; i < _singleMapping.Count;)
			{
				writer.Write($"\t\t");

				for (int j = 0; j < 4 && i < _singleMapping.Count; ++j, ++i)
				{
					var (bytes, size) = UnicodeToUtf8(_singleMapping[i].Item2);
					writer.Write($"{{ {{ 0x{bytes[0]:X2}, 0x{bytes[1]:X2}, 0x{bytes[2]:X2}, 0x{bytes[3]:X2} }}, {size} }}, ");
				}


				writer.WriteLine();
			}

			writer.WriteLine("\t};");



			writer.WriteLine("}");
		}
	}

	public static (byte[], uint) UnicodeToUtf8(uint ch)
	{
		byte[] res = new[]{ (byte)0, (byte)0, (byte)0, (byte)0 };
		uint size = 0;

		if (ch <= 0x7F)
		{
			res[0] = (byte)ch;
			size = 1;
		}
		else if (ch <= 0x7FF)
		{
			res[0] = (byte)(0xC0 | ((ch >> 6) & 0x1F));
			res[1] = (byte)(0x80 | (ch & 0x3F));
			size = 2;
		}
		else if (ch <= 0xFFFF)
		{
			res[0] = (byte)(0xE0 | ((ch >> 12) & 0x0F));
			res[1] = (byte)(0x80 | ((ch >> 6) & 0x3F));
			res[2] = (byte)(0x80 | (ch & 0x3F));
			size = 3;
		}
		else
		{
			res[0] = (byte)(0xF0 | ((ch >> 18) & 0x07));
			res[1] = (byte)(0x80 | ((ch >> 12) & 0x3F));
			res[2] = (byte)(0x80 | ((ch >> 6) & 0x3F));
			res[3] = (byte)(0x80 | (ch & 0x3F));
			size = 4;
		}
		return (res, size);
	}

}