
#include "BRZ_Config.h"


const BRZSTRING		BRZ::Config::BAD_STRING		= L"BAD_CONFIG_STRING";
const signed int	BRZ::Config::BAD_INT		= 0;
const float			BRZ::Config::BAD_FLOAT		= 0.0f;


const float & BRZ::Config::ReadFloat(const BRZSTRING & A_var) const
{
	auto result = fltVars.find(A_var);

	if (result == fltVars.cend())
		return BAD_FLOAT;
	else
		return result->second;
}


const signed int & BRZ::Config::ReadInt(const BRZSTRING & A_var) const
{
	auto result = intVars.find(A_var);

	if (result == intVars.cend())
		return BAD_INT;
	else
		return result->second;
}


const BRZSTRING & BRZ::Config::ReadString(const BRZSTRING & A_var) const
{
	auto result = strVars.find(A_var);

	if (result == strVars.cend())
		return BAD_STRING;
	else
		return result->second;
}


void BRZ::Config::Note(const BRZSTRING & A_msg)
{
	BRZSTRING msg = BRZSTRING(L"[Config]: ") + A_msg;

	log << BRZ::Narrow(msg) << std::endl;
	log.flush();
}


bool BRZ::Config::VarExists(const BRZSTRING & A_name)
{
	if (strVars.find(A_name) != strVars.cend() || intVars.find(A_name) != intVars.cend() || \
		fltVars.find(A_name) != fltVars.cend())
		return true;
	else
		return false;
}


BRZRESULT BRZ::Config::ParseLine(const BRZSTRING & A_line)
{
	const BRZSTRING		whitespace = L"\n\t ";
	const BRZSTRING		comment = L"//";
	const BRZSTRING		validInt = L"-0123456789";
	const BRZSTRING		validNeg = L"-";
	const BRZSTRING		validFloat = L"-.0123456789";
	const BRZSTRING		validDecimal = L".";
	const BRZSTRING		validEquiv = L"=";
	const BRZSTRING		validString = L"\"";
	BRZSTRING			line = A_line;
	BRZSTRING			name = L"";
	BRZSTRING			strArg = L"";
	signed int			intArg = 0;
	float				fltArg = 0;
	std::wistringstream	token;

	BRZ::TrimString(line);
	if (line.empty())
		return BRZ_SUCCESS;

	// Check if the line is a comment:
	if (line.length() < 2)
	{
		Note(L"Invalid line length detected.");
		return BRZ_FAILURE;
	}
	else if (line.substr(0, 2) == comment)
		return BRZ_SUCCESS;

	// Split off the variable name from the rest of the string:
	BRZSTRING::size_type	nameSplit = line.find_first_of(whitespace);
	name = line.substr(0, nameSplit);
	line = line.substr(nameSplit + 1, line.length() - (nameSplit + 1));
	BRZ::TrimString(line);

	// Make sure a variable with this name is not already defined:
	if (VarExists(name))
	{
		Note(L"Name collision detected.");
		return BRZ_FAILURE;
	}

	// Verify the argument string looks as it should: "= ...."
	if (line.empty())
	{
		Note(L"Empty argument string detected.");
		return BRZ_FAILURE;
	}
	if (line.substr(0, 1) != validEquiv)
	{
		Note(L"Invalid argument string detected -- no equivalence character.");
		return BRZ_FAILURE;
	}

	// Remove the equals sign, trim the argument string again:
	line = line.substr(1, line.length() - 1);
	BRZ::TrimString(line);

	// Verify the argument string is not empty:
	if (line.empty())
	{
		Note(L"Empty argument string after equivalence detected.");
		return BRZ_FAILURE;
	}


	// Test if the variable is string type: "text"
	BRZSTRING::size_type	strStart = line.find_first_of(validString);
	BRZSTRING::size_type	strEnd = line.find_last_of(validString);

	// Look for the quotation marks at the beginning and end of the argument:
	if (strStart == 0 && strEnd == line.length() - 1)
	{
		strArg = line.substr(1, line.length() - 2);
		std::pair<BRZSTRING, BRZSTRING>	var(name, strArg);
		strVars.insert(var);
		return BRZ_SUCCESS;
	}


	// Test if the variable is integer type:
	if (line.find_first_not_of(validInt) == BRZSTRING::npos)
	{
		// Verify that the negative sign, if present, only occurs once at the beginning:
		if (line.find_first_of(validNeg) != BRZSTRING::npos)
		{
			if (line.find_first_of(validNeg) != 0 || line.find_last_of(validNeg) != 0)
			{
				Note(L"Invalid integer argument detected -- multiple negatives.");
				return BRZ_FAILURE;
			}
		}

		token.str(line);
		token >> intArg;

		if (token.fail())
		{
			Note(L"Failed to read integer argument from ISS");
			return BRZ_FAILURE;
		}

		std::pair<BRZSTRING, signed int> var(name, intArg);
		intVars.insert(var);
		return BRZ_SUCCESS;
	}


	// Test if the variable is of floating point type:
	if (line.find_first_not_of(validFloat) == BRZSTRING::npos)
	{
		// Verify the negative sign exists (if it does) only once, at the start:
		if (line.find_first_of(validNeg) != BRZSTRING::npos)
		{
			if (line.find_first_of(validNeg) != 0 || line.find_last_of(validNeg) != 0)
			{
				Note(L"Invalid floating point argument detected -- multiple negatives.");
				return BRZ_FAILURE;
			}
		}

		// Verify the decimal point occurs only once:
		if (line.find_first_of(validDecimal) != line.find_last_of(validDecimal))
		{
			Note(L"Invalid floating point argument detected -- multiple decimal points.");
			return BRZ_FAILURE;
		}

		token.str(line);
		token >> fltArg;

		std::pair<BRZSTRING, float>	var(name, fltArg);
		fltVars.insert(var);
		return BRZ_SUCCESS;
	}

	// Could not parse argument, unknown type...
	Note(L"Invalid argument -- could not detect type.");
	return BRZ_FAILURE;
}


BRZRESULT BRZ::Config::LoadFile(const BRZSTRING & A_file)
{
	if (loaded)
	{
		Note(BRZSTRING(L"Configuration already loaded -- cannot load file: ") + A_file);
		return BRZ_FAILURE;
	}

	std::wifstream	input(A_file);

	if (!input.is_open())
	{
		Note(BRZSTRING(L"Failed to open configuration file: ") + A_file);
		return BRZ_FAILURE;
	}

	while (!input.eof())
	{
		BRZSTRING line = L"";
		std::getline(input, line);
		
		if (line.empty())
			continue;

		if (ParseLine(line) != BRZ_SUCCESS)
			Note(BRZSTRING(L"Failed to parse line: ") + line);
	}

	Note(L"Configuration loaded.");
	loaded = true;
	return BRZ_SUCCESS;
}


BRZ::Config::Config(std::ofstream & A_log) :
	log(A_log),
	loaded(false)
{
}


BRZ::Config::~Config()
{
}